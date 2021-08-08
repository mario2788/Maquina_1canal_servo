void F_servo(String opc_1)
{
  //int ang1_peq=95;//angulo inicial o neutro -->definido en programa principal (en el set up)
  int s=65;
  int ang2_peq;
  int x_peq=ang1_peq;//=102 (para el PowerPro)  //95 para el chino
  //
  int t_bajada_peq=3;//milisengundos
  int t_subida_peq=3;//milisengundos
  //
  //
  if (opc_1=="abajo")
  {
    ang2_peq=ang1_peq-s;//-60 para el chino
    for (ang1_peq;ang1_peq>=ang2_peq;ang1_peq-=paso)
          {
          ServoPeq.write(ang1_peq); // ang1 disminuye hasta ang2=35
          delay(t_bajada_peq);
          }
    ang1_peq=x_peq;//inicializamos ang1_peq (en el for llegó a valer ang2_peq)
    delay(150);
    ServoPeq.write(ang1_peq);
  }
  else if (opc_1=="arriba")
  {
    ang2_peq=ang1_peq+s;//
    for (ang1_peq;ang1_peq<=ang2_peq;ang1_peq+=paso)
          {
          ServoPeq.write(ang1_peq);
          delay(t_bajada_peq);
          }
    ang1_peq=x_peq;//inicializamos ang1_peq (en el for llegó a valer ang2_peq)
    delay(150);
    ServoPeq.write(ang1_peq); 
  }
  //***
  else if (opc_1=="arriba_rojo")
    {
    ang2_peq=ang1_peq+s;//
    for (ang1_peq;ang1_peq<=ang2_peq;ang1_peq+=paso)
          {
          ServoPeq.write(ang1_peq);
          delay(t_bajada_peq);
          }
    ang2_peq=x_peq;
    for (ang1_peq;ang1_peq>=ang2_peq;ang1_peq-=paso)
          {
          ServoPeq.write(ang1_peq); // ang1 disminuye hasta ang2=35
          delay(15);
          }
    ang1_peq=x_peq;//inicializamos ang1_peq (en el for llegó a valer ang2_peq) 
  }
}
