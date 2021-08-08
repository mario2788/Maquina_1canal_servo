#include<Servo.h> //al usar esta librería, se desactivan los PWM en pines 9 y_grande 10
#include <SoftwareSerial.h>
//
int a=0; //variable que nos permite salir del while y entrar al void loop (esperando que la celda avise que ya calibró)
char c='\0'; //para capturar los datos de la comunicación serial
String words; //guardamos la info que recibimos por comm serial
float cc=0;
char myArray[20] = "";
//
int ang1_grande=6;//angulo inicial clasificador
int ang1_peq=83;//angulo inicial o neutro de los servos pequeños
int paso=1; //deg (paso de los servos, tanto del grande como de los pequeños)
int ang_ini_grande;//angulo para que empiece a 90 grados antes de calibrar
int pinLed=10;
//
int M_in_rojo=13; //pin de digitalRead para cuando la celda nos avisa que se oprimió el botón rojo//ESTABA EL 8
int M_in_calibrada=12;//pin de digitalRead para cuando la celda nos avisa que ya fue calibrada (clasificador a posición de recibir huevos)//ESTABA 10
bool val_pin_rojo=false; //variable que identifica si la señal que manda la celda por oprimirle el botón rojo está activa
bool val_pin_calibrada=false;//variable que identifica si la celda ya está calibrada
int angxx=ang1_grande;
//
int xxx=0;
//
Servo ServoGrande; //objeto de la clase servo
Servo ServoPeq;
//
SoftwareSerial Serial_2 (3,4);//ESTABA (2,3)
//
void setup() 
{

  // put your setup code here, to run once:
  //Serial.begin(9600);
  //Serial.println("Máquina Pequeña V1");
  Serial_2.begin(9600);
  //
  ServoGrande.attach(6,500,2500); //HITEC
  ServoGrande.write(ang1_grande);//
  delay(400);
  ang_ini_grande=ang1_grande+90;//96°
      for (ang1_grande;ang1_grande<=ang_ini_grande;ang1_grande+=paso)
          {
          ServoGrande.write(ang1_grande);
          delay(5);
          }
  //ang1_grande=angxx;
  //
  ServoPeq.attach(7,500,2500); //GOTECK
  ServoPeq.write(ang1_peq);
  //
  pinMode(M_in_rojo,INPUT);
  pinMode(M_in_calibrada,INPUT);
  pinMode(pinLed,OUTPUT);
  digitalWrite(pinLed,LOW);
  //
  delay(50);
}
//
void loop() {
  
  while (a==0)
  {
    val_pin_calibrada=digitalRead(M_in_calibrada);
    if (val_pin_calibrada==true)
    {
      //Serial.println("Mecanismo recibió que la celda ya está calibrada");
      //ServoGrande.write(ang1_grande);//clasificador va a posición de recibimiento de huevos
      ang_ini_grande=angxx;
            for (ang1_grande;ang1_grande>=ang_ini_grande;ang1_grande-=paso)
            {
            ServoGrande.write(ang1_grande);
            delay(5);
            }
      a=1;
    }
  }

//  while (Serial_2.available()) // si hay info en el puerto serial:
//  { 
//    delay(5);
//    c=Serial_2.read(); //lee la info del puerto serial (c es char)
//    words +=c; //convierte a "c" en una cadena (string). también se puede escibir como words=words+c. se usa si digitamos en COM más de 1 caracter
//  }
//  delay(500); //tiempo para que el huevo se estabilice en el clasificador antes de rotar
  //
    //
  bool yy=false;
  while(yy==false){
  byte n = Serial_2.available();
  if (n != 0)
  {
    Serial_2.readBytesUntil('\n',myArray,20); //lee hasta '\n'
    cc = atof(myArray); //convierte ASCII a float
    //Serial.println(cc, 2);  //quita decimales
    char myArray[20] = ""; //reset array
    yy=true;
  }
      val_pin_rojo=digitalRead(M_in_rojo);
      if(val_pin_rojo==true){
        cc=99.99;
//        xxx=7;
//        yy=true;
        digitalWrite(pinLed,HIGH);
        break;
      }
  }
  delay(500); //tiempo para que el huevo se estabilice en el clasificador antes de rotar
  xxx=words.toInt();// transforma la cadena en un entero (recordar que "c" es char)
  //
    if (cc==99.99)//BOTÓN ROJO
    {
    digitalWrite(pinLed,HIGH);
    xxx=7; // 4A
    F_rueda(7);
    digitalWrite(pinLed,LOW);
    delay(1000);
    }
  else if (cc<=10 || cc>=130)
    {
    xxx=0; // no hay huevo o algo raro pasa
    }
  else if (cc<=45.9)
    {
    xxx=1; // C
    F_rueda(xxx);
    }
  else if (cc<=52.9)
    {
    xxx=2; // B
    F_rueda(xxx);
    }
  else if (cc<=59.9)
    {
    xxx=3; // A
    F_rueda(xxx);
    }
  else if (cc<=66.9)
    {
    xxx=4; // AA
    F_rueda(xxx);
    }
  else if (cc<=77.9)
    {
    xxx=5; // AAA
    F_rueda(xxx);
    }
  else if (cc>=78 && cc<130)
    {
    xxx=6; // 4A
    F_rueda(xxx);
    }
  //
  words="";//inicializamos la cadena
  //
}//cierre de void loop
