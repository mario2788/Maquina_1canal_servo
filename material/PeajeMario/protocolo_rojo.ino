void protocolo_rojo(){
    lecturaA=digitalRead(P_lecturaA); //BOTÓN ROJO
      if (lecturaA==true || a==0)
      {
      digitalWrite(9,LOW); // prendemos 9 de arduino
      a=0;    //para quedar en while de esperar botón verde
      aaa=1;  //para salir de los whiles de los ciclos de trabajo normal
      digitalWrite(P_out_ciclo, LOW); //apagamos arrancador para celda
      }
}
