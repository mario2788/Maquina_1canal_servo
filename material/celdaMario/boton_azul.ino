void boton_azul(){
  float PesoPatron=0;//
  boolean _resume = false;
  while (_resume == false){
    LoadCell_B.update();
    LoadCell_B.tare();
      if (LoadCell_B.getTareStatus() == true) {
        //Serial.println("Tare complete");
        _resume = true; //solo cuando el tare se ha completado salimos del while 
      }
  }
  // luz que avise para poner la masa
  digitalWrite(C_out_ledPatron, HIGH); //prendemos luz para avisar que pongan el patrón en la celda
  //
  //Serial.println("Now, place your known mass on the loadcell.");
  //Serial.println("Then send the weight of this mass (i.e. 100.0) from serial monitor.");
  _resume = false;
  int sss=0;
  unsigned long mm=millis();
  boolean newDataAzul = 0;
  int x=0;
  while (sss==0){
    if (LoadCell_B.update()) newDataAzul = true;
    if (newDataAzul){
      if (millis() > mm + 1000){
        //LoadCell_B.update();
        PesoPatron = LoadCell_B.getData();
        //Serial.print("valor pesado de patrón");
        //Serial_Palas.println(PesoPatron);
        mm=millis();
          if (PesoPatron>=50.0 && PesoPatron<=55.8){
          x=x+1;//para que una vez llegue al peso, lo vuelva a leer para asegurar que la medida haya sido completamente estable
          if (x==3){
            sss=1; //nos salimos del while
          }
          
          }
      }
    }
  }
  //
  sss=0;
  float known_mass = 52.80; //valor del patrón (con gramera de oro 45.1)
  //
  LoadCell_B.refreshDataSet(); //refresh the dataset to be sure that the known mass is measured correct
  LoadCell_B.getNewCalibration(known_mass); //get the new calibration value
  //
  digitalWrite(C_out_ledPatron, LOW); //apagamos led para avisar que retiren el patrón en la celda
  a=1;
  digitalWrite(C_out_calibrada,HIGH); //manda señal a peaje de que ya calibró (peaje recibe en pin 5)
}
