//Celdas-V4-peso//con respecto a V3:
//1. eliminamos librería EEPROM.h
//2. agregamos variables long para contar tiempos de millis cuando se necesite (ej: cuánto dura el tare)
//4. sacamos señal de digitalWrite(C_out_ciclo,HIGH); del ciclo while cuando lee que peaje ya llegó arriba. lo anterior para establecer delays cuando sea necesario (sobre todo por la cuestión del tiempo que dura el huevo en salir)
//5. (el pesaje en calibración de celda con botón azul se mantuvo en 2000ms)
//--Celdas-V4-peso-1//con respecto a Celdas-V4-peso:
//1. dejamos en la librería X muestras por dato (porque todo empieza con calibración botón azul) y quitamos las dos de los extremos (la más alta y la más baja)
//--Celdas-V4-peso-2//con respecto a Celdas-V4-peso-1:
//reducimos tiempo de pesaje a 550ms
//mejoramos el pesaje en calibración botón verde: cuando supere los 40g, vuelva y haga otro ciclo (para asegurar de que no tome una medida rápdida)
// [para con 1 muestra por dato, la bajada del peaje dura demasiado, entonces puede haber un delay antes y después de enviar la señal de bajar el peaje ]
//--Celdas-V4-peso-3//con respecto a Celdas-V4-peso-2:
//como tenemos 1 muestra por dato, realizamos doble tarar para mejorar
//--Celdas-V4-peso-4//con respecto a Celdas-V4-peso-3:
//dejamos la anterior como una especie de copia de seguridad
//pero sobre todo vamos a tomar el tiempo de ciclo
//--Celdas-V4-peso-5//con respecto a Celdas-V4-peso-4:
//vamos a quitar el enviado por serial del peso cuando es menor a 10g a ver si es la transmisión de datos por serial lo que origina el pateo de los servos
//--Celdas-V4-peso-6//con respecto a Celdas-V4-peso-5:
//tomando lo del código anterior, como no tenemos cómo mirar los tares, vamos es a encender el led si se sale de 0.09 o de -0.09 
#include <HX711_ADC.h>
//#include <EEPROM.h>
#include <SoftwareSerial.h>
//pins:
const int HX711_dout_B = 0; //mcu > HX711 dout pin//ESTABA 6
const int HX711_sck_B = 1; //mcu > HX711 sck pin//ESTABA 7
//
int aaa=0; //variable para acceder a ciclo normal de operación
int bbb=0; //variable para whiles durante el ciclo normal de trabajo
//
int categoria=0;
//
//char c=0;
int a=0;
//pines digital
int C_in_ciclo=9; //pin de digitalRead para ciclo normal de trabajo celda - peaje//ESTABA 8
int C_out_ciclo=10;//pin de digitalWrite para ciclo normal de trabajo celda - peaje
int C_in_azul=2;//pin de digitalRead para cuando se oprime el botón azul
int C_out_calibrada=11;//pin de digitalWrite para avisar a peaje que la celda ya fue calibrada//PUEDE MANEJARSE EN PARALELO PARA AVISAR A MECANISMO
int C_out_ledPatron=12;//pin de digitalWrite para prender luz para poner el patrón durante la calibración
int C_in_interrupt=3; //pin de interrupción para botón rojo
int C_out_rojoMecanismo=8;//pin de digitalWrite para avisar a Mecanismo que se oprimió el botón rojo//PUEDE MANEJARSE EN PARALELO CON PIN 13//CAMBIO AL PIN 8
//int C_out_stopPeaje=13;//pin de digitalWrite para avisar a Peaje que se oprimió el botón rojo //PUEDE MANEJARSE EN PARALELO CON PIN 9
int led_celda=13;
//
int ledState = HIGH; //valor para blink botón azul cuando se enciende la máquina
volatile int reinicio_loop=LOW;//para para que la interrupción haga que se reinicie el código
//
HX711_ADC LoadCell_B(HX711_dout_B, HX711_sck_B);//celda B
SoftwareSerial Serial_Palas (6,7);///REVISAR LA CELDA B
//
unsigned long t;
unsigned long antes_t=0;
unsigned long despues_t;
bool val=false;
bool azul = false;
byte estado_led_tare=LOW;
//
const long intervalo_blink = 200;
unsigned long previousMillis = 0;
void setup() 
{
  Serial_Palas.begin(9600);
  //
  pinMode(C_out_ciclo,OUTPUT);
  digitalWrite(C_out_ciclo,LOW);
  //
  pinMode(C_out_calibrada,OUTPUT); //11
  digitalWrite(C_out_calibrada,LOW); 
  //
  pinMode(C_in_ciclo,INPUT);// 8
  //
  pinMode(C_out_ciclo,OUTPUT);// 10
  digitalWrite(C_out_ciclo,LOW);
  //
  pinMode(C_in_azul,INPUT);// 2
  //
  pinMode(C_out_ledPatron,OUTPUT); // 12
  digitalWrite(C_out_ledPatron,HIGH);
  //
  pinMode(C_out_rojoMecanismo,OUTPUT); //9
  digitalWrite(C_out_rojoMecanismo,LOW);
  //
  pinMode(led_celda,OUTPUT); //9
  digitalWrite(led_celda,LOW);
  //
  //INTERRUPCIÓN PARA BOTÓN ROJO:
  pinMode(C_in_interrupt,INPUT); //entrada de botón verde para interrupt
  attachInterrupt(digitalPinToInterrupt(C_in_interrupt), boton_rojo, RISING);
  //
  // INICIALIZACIÓN CELDA:
  LoadCell_B.begin();
  float calibrationValue; // calibration value (see example file "Calibration.ino")
  calibrationValue = 964.16;//1834.12;//898.24 ; // uncomment this if you want to set the calibration value in the sketch celda original: 1154.0 //celda2, la soldó mario después -1658.63//celda 3 - después de ir a casa mario: 918.69
  //
  delay(150);
  digitalWrite(C_out_ledPatron,LOW);
  long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell_B.start(stabilizingtime, _tare);
//  if (LoadCell_B.getTareTimeoutFlag()) {
//    //Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
//    for (int ii=1;ii<=3;ii++){
//    digitalWrite(led_celda,HIGH);
//    delay(500);
//    digitalWrite(led_celda,LOW);
//    delay(500); 
//    }
//    while (1);
//  }
//  else {
    LoadCell_B.setCalFactor(calibrationValue); // set calibration value (float)
    //Serial.println("Startup is complete");
    digitalWrite(led_celda,HIGH);
    delay(100);
    digitalWrite(led_celda,LOW);
//  } 

}
//cierre set up

void loop() {
//*****toma del tiempo de ciclo
//despues_t=millis();
//despues_t=despues_t-antes_t;
//Serial_Palas.println(despues_t);//
//antes_t=millis();
//*****toma del tiempo de ciclo
  reinicio_loop=LOW;
  //
  while(a==0){
          unsigned long currentMillis = millis();
          if (currentMillis - previousMillis >= intervalo_blink) 
            {
            previousMillis = currentMillis;
            ledState=!ledState;
            digitalWrite(C_out_ledPatron, ledState);
            }
    azul=digitalRead(C_in_azul);
    if (azul==true)
      {
        boton_azul();
      }
    //
  }
  //-------------------inicio ciclo normal de trabajo---------------
  //
  if (a==1){ // solo entra si desde función botón azul tenemos un a=1

    static boolean newDataReadyDos = 0;
    const int serialPrintIntervalDos = 740;

    // check for new data/start next conversion:
    LoadCell_B.update();
      //ciclo de subida compleatado y hago tare: 
    while (bbb==0){
      val=digitalRead(C_in_ciclo);             //
      if(val==true){                  // si le llega un HIGH del peaje diciendo que ya está arriba
        digitalWrite(C_out_ciclo,HIGH);    //enviamos nuestra señal de celda para que el peaje baje (mientras baja haremos tare)
        bbb=1;                    // para salirnos del while
      }
      if (reinicio_loop==HIGH){
          delay(1500);
          digitalWrite(C_out_rojoMecanismo,LOW);
          //break;//para que no se me salga de este while (while (bbb==0))
          reinicio_loop=LOW;
        }
    }
    //
    LoadCell_B.tare();//tare 1
    delay(250);
    LoadCell_B.tare();//tare 2
    //
    bbb=0; //inicializamos esta variable para usarla en el ciclo de bajada
    //
    if(estado_led_tare==HIGH){//en caso de que el tare esté mal, acá vamos a apagar el led para que no se quede prendido
      estado_led_tare=LOW;
    digitalWrite(led_celda,estado_led_tare);
       }
    digitalWrite(C_out_ciclo,HIGH);    //enviamos nuestra señal de celda para que el peaje baje (mientras baja haremos tare)
    bbb=0; //inicializamos esta variable para usarla en el ciclo de bajada
    //
    //ciclo de bajada completado y hago el pesaje:
    while (bbb==0){
      val=digitalRead(C_in_ciclo);             // llega señal de pinOut peaje de que pese
      if(val==false){                  // si le llega un HIGH del peaje diciendo que ya bajó
        bbb=1;                    // no hacemos nada, simplemente nos salimos del while para seguir bajando en el código y hacer el pesaje
      }
    }
    bbb=0; //inicializamos esta variable para usarla en el ciclo de bajada
    //
    t=millis();
    // PESAJE
    //digitalWrite(led_celda,HIGH);
    while (aaa==0){
      if (LoadCell_B.update()) newDataReadyDos = true;
          if (newDataReadyDos == true)
            {
              if (millis() > t + serialPrintIntervalDos)
              {
                float i = LoadCell_B.getData(); //hacemos el pesaje
                digitalWrite(C_out_ciclo,LOW); //una vez hecho el pesaje, le avisamos al peaje de que ya pesamos, que ya puede subir
                t = millis();
                //
                  if (reinicio_loop==HIGH){
                  delay(1500);
                  digitalWrite(C_out_rojoMecanismo,LOW);
                  break;
                }
                if(i>=10){
                Serial_Palas.println(i);//vamos a mirar si es la comunicación serial la que origina el pateo del servo (pues cuando estamos en botón azul o rojo nunca patean)
                }
                else if(i>=0.09 || i<=-0.09){//verificación de que el tare sí está quedando bien
                  estado_led_tare=HIGH;
                  digitalWrite(led_celda,estado_led_tare);
                }
                //
                aaa=1;//con este aaa=1 ya se puede salir del while (pues aprobó el último if)
              }//FIN "if (millis() > t + serialPrintIntervalDos)"
            } //FIN "if (newDataReadyDos == true)" 

    }//FIN WHILE PESAJE
    aaa=0;
    //
    //newDataReady = 0;
    newDataReadyDos = 0;
    //
    //digitalWrite(led_celda,LOW);
    
  } //cierre de "if (a==1)"
  //-------------------fin ciclo normal de trabajo---------------
  //
} // cierre void loop
