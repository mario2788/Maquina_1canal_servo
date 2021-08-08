void F_rueda(int categoria)
{
  int ang2_grande=7;//angulo final peaje
  int t_rot_grande=6;//milisengundos
  int x_grande=ang1_grande;
  //
if (categoria==1 || categoria==6)//categoría C y J
  {
    ang2_grande=ang1_grande+6;//12°
    for (ang1_grande;ang1_grande<=ang2_grande;ang1_grande+=paso)
    {
      ServoGrande.write(ang1_grande);
      delay(t_rot_grande);
    }
    F_servo("arriba");
    ang2_grande=x_grande;
  for (ang1_grande;ang1_grande>=ang2_grande;ang1_grande-=paso)
    {
      ServoGrande.write(ang1_grande);
      delay(t_rot_grande);
    }
  ang1_grande=x_grande; //inicializamos ang1_grande por si las moscas
  }
else if (categoria==2 )//categoría B
{
  ang2_grande=ang1_grande+49;//55°
  for (ang1_grande;ang1_grande<=ang2_grande;ang1_grande+=paso)
    {
      ServoGrande.write(ang1_grande);
      delay(t_rot_grande);
    }
    F_servo("arriba");
  ang2_grande=x_grande;
  for (ang1_grande;ang1_grande>=ang2_grande;ang1_grande-=paso)
    {
      ServoGrande.write(ang1_grande);
      delay(t_rot_grande);
    }
  ang1_grande=x_grande; //inicializamos ang1_grande por si las moscas
}
else if (categoria==3) //categoría A
{
  ang2_grande=ang1_grande+87;//93°
  for (ang1_grande;ang1_grande<=ang2_grande;ang1_grande+=paso)
    {
      ServoGrande.write(ang1_grande);
      delay(5);
    }
      F_servo("abajo");
  ang2_grande=x_grande;
  for (ang1_grande;ang1_grande>=ang2_grande;ang1_grande-=paso)
    {
      ServoGrande.write(ang1_grande);
      delay(5);
    }
  ang1_grande=x_grande; //inicializamos ang1_grande por si las moscas
}
else if (categoria==4) //categoría AA
{
  ang2_grande=ang1_grande+94;//100°
  for (ang1_grande;ang1_grande<=ang2_grande;ang1_grande+=paso)
    {
      ServoGrande.write(ang1_grande);
      delay(5);
    }
      F_servo("arriba");
  ang2_grande=x_grande;
  for (ang1_grande;ang1_grande>=ang2_grande;ang1_grande-=paso)
    {
      ServoGrande.write(ang1_grande);
      delay(5);
    }
  ang1_grande=x_grande; //inicializamos ang1_grande por si las moscas
}
else if (categoria==5)//categoría AAA
{
  ang2_grande=ang1_grande+131;//137°
  for (ang1_grande;ang1_grande<=ang2_grande;ang1_grande+=paso)
    {
      ServoGrande.write(ang1_grande);
      delay(3);
    }
  F_servo("abajo");
  ang2_grande=x_grande;
  for (ang1_grande;ang1_grande>=ang2_grande;ang1_grande-=paso)
    {
      ServoGrande.write(ang1_grande);
      delay(3);
    }
  ang1_grande=x_grande; //inicializamos ang1_grande por si las moscas
}
else if (categoria==7)
{
  ang2_grande=ang1_grande+0;
  for (ang1_grande;ang1_grande<=ang2_grande;ang1_grande+=paso)
    {
      ServoGrande.write(ang1_grande);
      delay(t_rot_grande);
    }
  F_servo("arriba_rojo");
  ang1_grande=x_grande; //inicializamos ang1_grande, pues en el for varió su valor
  ServoGrande.write(ang1_grande);//retornamos la rueda a cero
}
} //cierre función rueda
