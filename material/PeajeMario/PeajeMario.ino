//Peaje_V2_negro-2//con respecto a Peaje_V2_negro-1
//se hace una transición lenta de 8 a 1 segundos en la subida
//Peaje_V2_negro-3//con respecto a Peaje_V2_negro-2
//aumentamos 5 grados en subida a 8ms para una expulsión menos fuerte del huevo
//disminuinmos el ciclo de 110 a 106 grados para ang1_peaje
//Peaje_V2_negro-6//con respecto a Peaje_V2_negro-5
//establecemos variable para led de micro peaje 
#include<Servo.h>
int ang1_peaje=45;//angulo inicial servo -->empieza arriba -- 24dic:estaba en 0
int aaa=0;
int bbb=0;
//pines
int P_out_ciclo=7; //pin de digitalWrite para ciclo normal de trabajo peaje - celda
int P_in_ciclo=6; //pin de digitalRead para ciclo normal de trabajo peaje - celda//ESTABA 4
int P_in_verde=2; //pin de digitalRead para cuando se oprime el botón verde
int P_in_calibrada=5; //pin de digitalRead cuando la celda avisa que ya fue calibrada
int P_lecturaA = 8; //pin de digitalRead cuando la celda avisa tuvo botón rojo, así tiene que volver al ciclo while, es decir, volver a darle el verde//ESTABA 11
int P_led_peaje=9;//pin led micro peaje
//
bool val=false;
bool val_calibracion=false;
bool calibrada_hecha=false;
bool val_verde=false;
//
int a=0;
bool verde=false;
bool lecturaA=false;
//
Servo ServoPeaje; //objeto de la clase servo
//
void setup() 
{
  ServoPeaje.attach(11,500,2500); //anclamos en pin #4, 650 y 2650 us (microsegundos de calibración POLOLU)//ESTABA EL 9
  ServoPeaje.write(ang1_peaje); //empieza en este caso arriba
  //
  pinMode(P_in_verde,INPUT);
  // 
  pinMode(P_in_ciclo,INPUT);
  //
  pinMode(P_out_ciclo,OUTPUT);
  digitalWrite(P_out_ciclo, LOW);
  //
  pinMode(P_lecturaA, INPUT);
  //
  pinMode(P_led_peaje, OUTPUT);  
  //
  digitalWrite(P_led_peaje,HIGH);//verificación de tarjeta
  delay(500);
  digitalWrite(P_led_peaje,LOW);
  
}
//cierre set up
void loop() {
  //
  while(a==0)
  {
    val_calibracion=digitalRead(P_in_calibrada); 
    if(val_calibracion==true) //solo si la celda ya fue calibrada entramos a la función botón verde
    {
      boton_verde(); //solo si la celda ya fue calibrada entramos a la función botón verde
    }
  }
  //----------------------ciclo de trabajo normal------------------------------------------
  if(a==1){ //solo ejecuta todo el void loop si la variable 'a' que viene de función boton_verde está en 1
  //
    while (aaa==0)
    {
      val=digitalRead(P_in_ciclo);       //llega señal del pinOut 10 para bajar peaje (mientras la celda va a tarar)
      if(val==true)                       // si le llega un HIGH de la celda
      {            
        CicloServo("bajada");   // hace su ciclo
        digitalWrite(P_out_ciclo, LOW);  //como acabó de bajar el peaje, también manda su señal para que la celda haga el pesaje
        aaa=1;                    // para salirnos del while
      }
    }
    aaa=0;
    //
    while (aaa==0)
    {
      val=digitalRead(P_in_ciclo);
      if(val==false)                //si le llega un LOW de la celda
      {                  
        CicloServo("subida");     // hace su ciclo de subida
        digitalWrite(P_out_ciclo, HIGH);  //al acabar su ciclo (peaje arriba), manda señal a celda de que tare
        aaa=1;                    // para salirnos del while
      }
    }
    aaa=0;
  } // fin de "if(a==1)"
  //-----------------------------fin de ciclo normal de trabajo-----------------------------------
  //
  lecturaA=digitalRead(P_lecturaA); //BOTÓN ROJO
  if (lecturaA==true)
  {
    a=0;////para quedar en while de esperar botón verde
    digitalWrite(P_out_ciclo, LOW); //apagamos arrancador para celda
    //digitalWrite(9,LOW); // prendemos 9 de arduino
  }
}
