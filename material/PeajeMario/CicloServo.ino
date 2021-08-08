void CicloServo(String opc_peaje)
{
  //int ang1_peaje=50;//en el negro con peaje estándar era de 106 a 178, o sea 72 grados
  int ang2_peaje=ang1_peaje+95;//142//en el negro con peaje estándar 178
  int t_bajada_peaje=8;//milisengundos
  int t_subida_peaje=8;//milisengundos
  int x_peaje=ang1_peaje;
  int y_peaje=ang2_peaje;
  int paso=1;
  //
  int ciclo1_bajada = ang1_peaje + 10;//62 - ciclo 1 con negro y peaje convencional: 125
//  int ciclo1x1_bajada = ang1_peaje + 23;//67 - ciclo 1 con negro y peaje convencional: 125
//  int ciclo1x2_bajada = ang1_peaje + 26;//72 - 
//  int ciclo1x3_bajada = ang1_peaje + 40;//72 -
  int ciclo1x1_bajada = ang1_peaje + 26;//72 - ciclo 1 con negro y peaje convencional: 125
  int ciclo3_bajada = ang1_peaje + 57;//107 - ciclo 3 con negro y peaje convencional: 140
  int ciclo4_bajada = ang1_peaje + 77;//125 - ciclo 4 con negro y peaje convencional: 160
  //
  int ciclo1_subida = ang2_peaje - 34;//103 - ciclo 1 con negro y peaje convencional: 146
  //
// ------------------------------------------------
// bajada de PEAJE
if (opc_peaje=="bajada") //se intercambiaron las desigualdades en ambos casos para Celda-MaqPeqV6_20Sept pues intercambiamos valores de ang1 y ang2
{
  for (ang1_peaje;ang1_peaje<=ang2_peaje;ang1_peaje+=paso) //aumentamos ang1 de 0 a 95
    {
         if (ang1_peaje<=ciclo1_bajada){t_bajada_peaje=8;}//arranque lento para tomar huevo - 
         //else if (ang1_peaje<=ciclo1x1_bajada){t_bajada_peaje=5;}//arranque lento parte 2para tomar huevo -
         else if (ang1_peaje<=ciclo1x1_bajada){t_bajada_peaje=5;}//arranque lento parte 3 para tomar huevo -
         else if (ang1_peaje>ciclo3_bajada && ang1_peaje<=ciclo4_bajada){t_bajada_peaje=10;}
         else if (ang1_peaje>ciclo4_bajada){t_bajada_peaje=2;}
         else t_bajada_peaje=3;
    ServoPeaje.write(ang1_peaje);
    delay(t_bajada_peaje);
    }
  ang1_peaje=x_peaje;//inicializamos ang1_peaje (en el for llegó a valer ang2_peaje)
}
//// ------------------------------------------------
// subida de PEAJE
else if (opc_peaje=="subida")
{
  for (ang2_peaje;ang2_peaje>=ang1_peaje;ang2_peaje-=paso) //disminuimos ang2 de 142 a 50
    {
         if (ang2_peaje>=ciclo1_subida){t_subida_peaje=10;}
         else {
          t_subida_peaje--;
            if (t_subida_peaje<1){
              t_subida_peaje=1;
              }
         }
    ServoPeaje.write(ang2_peaje);
    delay(t_subida_peaje);
    }
  ang2_peaje=y_peaje;//inicializamos ang2_peaje (en el for llegó a valer ang1_peaje)
  //
  delay(100);//este delay es para que el huevo por gravedad llegue al tope del peaje
  digitalWrite(P_out_ciclo, HIGH);  //al acabar su ciclo (peaje arriba), manda señal a celda de que tare
  delay(215);//este delay es para que el huevo por gravedad llegue al tope del peaje
}
}
