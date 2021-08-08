void boton_verde(){
  a=0;
  while(a==0)
  {
//    val_calibracion=digitalRead(P_in_calibrada); //celda manda una señal digital para avisar a peaje si ya se realizó calibrada
//    if(val_calibracion==true)
//    {
//      a=1; //valor para entrar a ciclo normal de trabajo
//    }
    //***
    verde=digitalRead(P_in_verde);
    if (verde==true) //solo si se oprime el botón verde
    {
      a=1;//si se oprime botón verde "a" se vuelve 1, podremos salir del while y arrancar el ciclo de trabajo normal
        //verificación de que está leyendo el botón verde
        digitalWrite(P_led_peaje,HIGH);
        delay(500);
        digitalWrite(P_led_peaje,LOW);
        //
    }
    //***
  }
  digitalWrite(P_out_ciclo, HIGH); //arrancador para celda
}
