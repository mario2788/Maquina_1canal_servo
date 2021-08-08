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
      BTN_ROJO          Pin_7   -|              |-   Pin_10      SERVO_EXPUL
      BTN_AZUL          Pin_8   -|______________|-   Pin_9       SERVO_CLASIFI

*/
// dar privilegios al conversor Serial
// sudo chmod ugo+x+r+w /dev/ttyUSB0
#include <Arduino.h>
#include <Servo.h>

//Funciones externas
#include "leds_btns.cpp"
#include "celda.cpp"


// Servos:
Servo SERVO_PEAJE ;
Servo SERVO_CLASIFI ;
Servo SERVO_EXPUL ;


//Posiciones de los motores:
// Al prender:
const int POS_PEAJE_ON = 90 ;
const int POS_CLASIFI_ON = 120 ;
const int POS_EXPUL_ON = 140 ;
// Tras el oprimir el boton verde
const int POS_PEAJE_ON = 90 ;
const int POS_CLASIFI_ON = 120 ;
const int POS_EXPUL_ON = 140 ;

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
   SERVO_PEAJE.attach   ( 5, 500, 2500 );
   SERVO_CLASIFI.attach ( 9, 500, 2500 );
   SERVO_EXPUL.attach   ( 10, 500, 2500);
   SERVO_PEAJE.write ( POS_PEAJE_ON ) ;
   SERVO_CLASIFI.write( POS_CLASIFI_ON ) ;
   SERVO_EXPUL.write ( POS_EXPUL_ON ) ;

   // Celda de carga:
   pinMode( LOADCELL_SCK_PIN, OUTPUT );
   pinMode( LOADCELL_DOUT_PIN, INPUT );

   // Iniciar sensor
   bascula.begin( LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

   //variables:
   delay( 100 ) ;
   digitalWrite( BTN_AZUL, HIGH ) ;
   digitalWrite( BTN_VERDE, HIGH ) ;
   digitalWrite( BTN_ROJO, HIGH ) ;

   // calibracion de la celda
   calibrarCelda() ;
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
