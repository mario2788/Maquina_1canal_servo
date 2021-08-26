/*                                ______________
                        Reset   -|              |-   Analog_5
      RX: PC/WIFI       Pin_0   -|      A       |-   Analog_4
      TX: PC/WIFI       Pin_1   -|      T       |-   Analog_3
      LED_AZUL          Pin_2   -|      M       |-   Analog_2
      LED_VERDE         Pin_3   -|      E       |-   Analog_1
      LED_ROJO          Pin_4   -|      G       |-   Analog_0
                        VCC     -|      A       |-   GND
                        GND     -|      3       |-   AREF
                        CR_16MHz-|      2       |-   AVCC
                        CR_16MHz-|      8       |-   Pin_13      SCK
      SERVO_PEAJE       Pin_5   -|      A       |-   Pin_12      MISO
      BTN-VERDE         Pin_6   -|              |-   Pin_11      MOSI
      BTN_ROJO          Pin_7   -|              |-   Pin_10      SERVO_EXPULSION
      BTN_AZUL          Pin_8   -|______________|-   Pin_9       SERVO_CLASIFICADOR

   Motor mecanismo de admisión huevo a huevo ->    motor peaje
	Motor mecanismo de rotación vertical      ->    motor clasificador
	Motor mecanismo de expulsión de huevo     ->    micromotor

*/
// dar privilegios al conversor Serial
// sudo chmod ugo+x+r+w /dev/ttyUSB0
#include <Arduino.h>
#include <Servo.h>
#include <TimerOne.h>

//Funciones externas
#include "leds_btns.cpp"
#include "celda.cpp"

// Ciclos para el motor de peaje:
// Parametros: 
const int CICLO_PEAJE_1 [] = {1222, 1224, 1229, 1239, 1251, 1268, 1287, 1309, 1333, 1359, 1386, 1415, 1444, 1444, 1473, 1501, 1529, 1555, 1579, 1601, 1620, 1636, 1649, 1659, 1664, 1666} ;
const int CICLO_PEAJE_2 [] = {1222, 1224, 1229, 1239, 1251, 1268, 1287, 1309, 1333, 1359, 1386, 1415, 1444, 1444, 1473, 1501, 1529, 1555, 1579, 1601, 1620, 1636, 1649, 1659, 1664, 1666} ;
const int CICLO_PEAJE_3 [] = {1222, 1224, 1229, 1239, 1251, 1268, 1287, 1309, 1333, 1359, 1386, 1415, 1444, 1444, 1473, 1501, 1529, 1555, 1579, 1601, 1620, 1636, 1649, 1659, 1664, 1666} ;
// Ciclos para el motor de CLASIFI:
const int CICLO_CLASIFI_1 [] = {1222, 1224, 1229, 1239, 1251, 1268, 1287, 1309, 1333, 1359, 1386, 1415, 1444, 1444, 1473, 1501, 1529, 1555, 1579, 1601, 1620, 1636, 1649, 1659, 1664, 1666} ;
const int CICLO_CLASIFI_2 [] = {1222, 1224, 1229, 1239, 1251, 1268, 1287, 1309, 1333, 1359, 1386, 1415, 1444, 1444, 1473, 1501, 1529, 1555, 1579, 1601, 1620, 1636, 1649, 1659, 1664, 1666} ;
const int CICLO_CLASIFI_3 [] = {1222, 1224, 1229, 1239, 1251, 1268, 1287, 1309, 1333, 1359, 1386, 1415, 1444, 1444, 1473, 1501, 1529, 1555, 1579, 1601, 1620, 1636, 1649, 1659, 1664, 1666} ;

// Servos:
Servo SERVO_PEAJE ;
Servo SERVO_CLASIFI ;
Servo SERVO_EXPUL ;


void ISR_servo(){   

}

// Posiciones de los motores:
// Al prender:
const int POS_PEAJE_ON = 90 ;
const int POS_CLASIFI_ON = 120 ;
const int POS_EXPUL_ON = 140 ;
// Tras el oprimir el boton verde
const int POS_PEAJE_LED_VERDE = 90 ;
const int POS_CLASIFI_LED_VERDE = 120 ;
const int POS_EXPUL_LED_VERDE = 140 ;

// Ciclos:
char CICLOS_PEAJE []    = { '1', '2', '3' } ;
char CICLOS_CLASIFI []  = { '1', '2', '3' } ;
char CICLOS_PEAJE []    = { '1', '2', '3' } ;

void setup() {
   // put your setup code here, to run once:

   // leds:
   pinMode( LED_AZUL,   OUTPUT );
   pinMode( LED_VERDE,  OUTPUT );
   pinMode( LED_ROJO,   OUTPUT );

   // botones:
   pinMode( BTN_AZUL,   INPUT ) ;
   pinMode( BTN_VERDE,  INPUT ) ;
   pinMode( BTN_ROJO,   INPUT ) ;
   digitalWrite( BTN_AZUL, HIGH ) ;
   digitalWrite( BTN_VERDE, HIGH ) ;
   digitalWrite( BTN_ROJO, HIGH ) ;

   // Serial:
   Serial.begin(115200);
   Serial.println("FirmWare With Servo motor");

   // Servos:
   SERVO_PEAJE.attach   ( 5, 500, 2500 ) ; // ( pin, min_ms, max_ms)
   SERVO_CLASIFI.attach ( 9, 500, 2500 ) ;
   SERVO_EXPUL.attach   ( 10, 500, 2500) ;
   SERVO_PEAJE.write ( POS_PEAJE_ON ) ;
   SERVO_CLASIFI.write( POS_CLASIFI_ON ) ;
   SERVO_EXPUL.write ( POS_EXPUL_ON ) ;
   
   // Celda de carga:
   pinMode( LOADCELL_SCK_PIN, OUTPUT );
   pinMode( LOADCELL_DOUT_PIN, INPUT );

   // Iniciar sensor
   bascula.begin( LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN );

   //variables:
   delay( 100 ) ;
   digitalWrite( BTN_AZUL, HIGH ) ;
   digitalWrite( BTN_VERDE, HIGH ) ;
   digitalWrite( BTN_ROJO, HIGH ) ;

   // calibracion de la celda
   calibrarCelda() ;

   // interrupcion por timer1
   Timer1.initialize(250000);         // Dispara cada 250 ms
   Timer1.attachInterrupt( ISR_servo ); // Activa la interrupcion y la asocia a ISR_Blink

}

// posicion de los servos en el test
int pos = 0;
void loop() {
   // put your main code here, to run repeatedly:

   // Test botones y leds:
   test_leds_btns() ;

   // test servo motores
   if( Serial.available() > 0 ){
      // read the incoming byte:
      char received = Serial.read();
      Serial.print( received );

      if( received == 'u' && pos+5 <= 180){
         pos = pos + 5 ;
         SERVO_PEAJE.write(pos);
         SERVO_CLASIFI.write(pos);
         SERVO_EXPUL.write(pos);
         Serial.println(pos, DEC);
      }

      if( received == 'd' && pos-5 >= 0){
         pos = pos - 5 ;
         SERVO_PEAJE.write(pos);
         SERVO_CLASIFI.write(pos);
         SERVO_EXPUL.write(pos);
         Serial.println(pos, DEC );
      }
      received = ' ';
      Serial.flush();
   }

   // Test celda de carga HX711.h
   float value = bascula.get_units(3); // Tres lecturas
   Serial.print( "Peso:" ) ;
   Serial.println( value ) ;
}
