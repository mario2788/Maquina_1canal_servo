void boton_rojo(){
  digitalWrite(C_out_ciclo,LOW); //para el peaje arriba, o igual hace que termine arriba
  digitalWrite(C_out_rojoMecanismo,HIGH); //envía señal para que mecanismo tome un xxx=1 y deje en la primera categoría 
  reinicio_loop = HIGH;
}
