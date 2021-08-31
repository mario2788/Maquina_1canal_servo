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
#include <TimerOne.h>

//Funciones externas
#include "leds_btns.cpp"
#include "celda.cpp"
#include "servos.cpp"

// Categoria de huevos:
const unsigned char a = 24 ;
const unsigned char aa = 35 ;
const unsigned char b = 40 ;
const unsigned char bb = 50 ;
// tolerancia: +-
const unsigned char tol = 5;

void ISR_servo() ;
unsigned char categoria(float value);

void setup() {

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
   SERVO_PEAJE.writeMicroseconds( POS_PEAJE_ON ) ;
   SERVO_CLASIFI.writeMicroseconds( POS_CLASIFI_ON ) ;
   SERVO_EXPUL.writeMicroseconds ( POS_EXPUL_ON ) ;
   
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
   // motor clasificador a 90°:
   SERVO_CLASIFI.write(90);
   calibrarCelda() ;

   // interrupcion por timer1
   Timer1.initialize( 500000 );         // Dispara cada 500 ms
   Timer1.attachInterrupt( ISR_servo ); // Activa la interrupcion y la asocia a ISR_Blink

}

void loop() {
   // put your main code here, to run repeatedly:

   // test_leds_btns() ;
   // test_servo();
   // test_celda()
   unsigned int value = 0 ;
   unsigned char len = sizeof(CICLO_PEAJE_2)-1 ;
   
   for ( unsigned int i = 0; i <= len ; i++ ){
      value = CICLO_PEAJE_2[ i ] ;
      SERVO_PEAJE.writeMicroseconds( value ) ;
      Serial.println( CICLO_PEAJE_2[ i ]  );
      Serial.println( i  );
      delay(500);
   }
   

   float value_celda = 0 ;
   unsigned char cat = 0 ;

   // Espera hasta que se oprima el boton verde
   Serial.println("En espera del boton verde");
   while( readBtn( BTN_VERDE, LOW ) ){
      Serial.println("En espera...");
   }

   //Inicio de la rutina de trabajo:

   // carga huevo a la celda:
   Serial.println("Carga del ciclo 1") ;
   CICLOS_PEAJE = 1 ; //Se da comienzo al movimiento.
   while( CICLOS_PEAJE > 0 ){
      Serial.print( "" );
   } 
   // espera a terminar el movimiento
   Serial.println("ciclo 1");

   delay(500); // Espera para estabilizacion del huevo.
   Serial.println("Inicio medida");
   value_celda = bascula.get_units(3); // Tres lecturas
   cat = categoria( value_celda ); // definicion de categoria

   // carga huevo al clasificador
   Serial.println("Carga del ciclo 2");
   CICLOS_PEAJE = 2 ; //Se da comienzo al movimiento.
   while( CICLOS_PEAJE > 0 ){
      Serial.print("");
   } 
   Serial.println("ciclo 2");

   Serial.println("Carga del ciclo 3");
   CICLOS_PEAJE = 3 ; //Se da comienzo al movimiento.
   while( CICLOS_PEAJE > 0 ){
      Serial.print("");
   } 
   Serial.println("ciclo 3");
   
}

unsigned char categoria(float value){

   unsigned char return_value = 0 ;
   
   if( value >= a + tol || value >= a - tol ){
      return return_value = 1 ;
   }

   if( value >= aa + tol || value >= aa - tol ){
      return return_value = 2 ;
   }     

   if( value >= b + tol || value >= b - tol ){
      return return_value = 3 ;
   }     

   if( value >= bb + tol || value >= b - tol ){
      return return_value = 4 ;
   }    
   
   return 0 ;
}

void ISR_servo(){   
   // administracion de ciclos del motor clasificador
   if( CICLOS_CLASIFI !=0 ){
      Serial.write("Llamado ciclo clasifi  \n");
      switch ( CICLOS_CLASIFI ){
         case 1:
            if( INDEX_CLASIFI <  sizeof(CICLO_CLASIFI_1)-1 ){
               SERVO_CLASIFI.writeMicroseconds( CICLO_CLASIFI_1[INDEX_CLASIFI] ) ;
               Serial.println( CICLO_CLASIFI_1[INDEX_CLASIFI] );
               INDEX_CLASIFI = INDEX_CLASIFI + 1 ;
            }else{
               CICLOS_CLASIFI = 0 ;
               INDEX_CLASIFI = 0;
            }
            break;
         case 2:
            if( INDEX_CLASIFI <  sizeof(CICLO_CLASIFI_2)-1 ){
               SERVO_CLASIFI.writeMicroseconds( CICLO_CLASIFI_2[INDEX_CLASIFI] ) ;
               Serial.println( CICLO_CLASIFI_2[INDEX_CLASIFI] );
               INDEX_CLASIFI = INDEX_CLASIFI + 1 ;
            }else{
               CICLOS_CLASIFI = 0 ;
               INDEX_CLASIFI = 0;
            }
            break; 
         case 3:
            if( INDEX_CLASIFI <  sizeof(CICLO_CLASIFI_3)-1 ){
               SERVO_CLASIFI.writeMicroseconds( CICLO_CLASIFI_3[INDEX_CLASIFI] ) ;
               Serial.println( CICLO_CLASIFI_3[INDEX_CLASIFI] );
               INDEX_CLASIFI = INDEX_CLASIFI + 1 ;
            }else{
               CICLOS_CLASIFI = 0 ;
               INDEX_CLASIFI = 0;
            }         
            break;      
         default:
            break;
         }
   }

   // administracion de ciclos del motor de expulsion
   if( CICLOS_EXPUL !=0 ){
      Serial.write("Llamado ciclo expul  \n");
      switch ( CICLOS_EXPUL ){
         case 1:
            if( INDEX_EXPUL <  sizeof(CICLO_EXPUL_1)-1 ){
               INDEX_EXPUL = INDEX_EXPUL + 1 ;
               SERVO_EXPUL.writeMicroseconds( CICLO_EXPUL_1[INDEX_EXPUL] ) ;
               Serial.println( CICLO_EXPUL_1[INDEX_EXPUL] );
            }else{
               CICLOS_EXPUL = 0 ;
               INDEX_EXPUL = 0;
            }
            break;
         case 2:
            if( INDEX_EXPUL <  sizeof(CICLO_EXPUL_2)-1 ){
               INDEX_EXPUL = INDEX_EXPUL + 1 ;
               SERVO_EXPUL.writeMicroseconds( CICLO_EXPUL_2[INDEX_EXPUL] ) ;
               Serial.println( CICLO_EXPUL_2[INDEX_EXPUL] );
            }else{
               CICLOS_EXPUL = 0 ;
               INDEX_EXPUL = 0;
            }
            break; 
         case 3:
            if( INDEX_EXPUL <  sizeof(CICLO_EXPUL_3)-1 ){
               INDEX_EXPUL = INDEX_EXPUL + 1 ;
               SERVO_EXPUL.writeMicroseconds( CICLO_EXPUL_3[INDEX_EXPUL] ) ;
            }else{
               CICLOS_EXPUL = 0 ;
               INDEX_EXPUL = 0;
            }         
            break;      
         default:
            break;
         }
   }

  // administracion de ciclos del motor peaje
   if( CICLOS_PEAJE !=0 ){
      Serial.println("Llamado ciclo peaje");
      switch ( CICLOS_PEAJE ){
         case 1:
            if( INDEX_PEAJE < sizeof(CICLO_PEAJE_1)-1 ){               
               SERVO_PEAJE.writeMicroseconds( CICLO_PEAJE_1[INDEX_PEAJE] ) ;
               Serial.println( CICLO_PEAJE_1[INDEX_PEAJE] );
               INDEX_PEAJE = INDEX_PEAJE + 1 ;
            }else{
               CICLOS_PEAJE = 0 ;
               INDEX_PEAJE = 0;
            }
            break;
         case 2:
            if( INDEX_PEAJE <  sizeof(CICLO_PEAJE_2)-1 ){
               SERVO_PEAJE.writeMicroseconds( CICLO_PEAJE_2[INDEX_PEAJE] ) ;
               Serial.println( CICLO_PEAJE_2[INDEX_PEAJE] );
               INDEX_PEAJE = INDEX_PEAJE + 1 ;
            }else{
               CICLOS_PEAJE = 0 ;
               INDEX_PEAJE = 0;
            }
            break; 
         case 3:
            if( INDEX_PEAJE <  sizeof(CICLO_PEAJE_3)-1 ){
               SERVO_PEAJE.writeMicroseconds( CICLO_PEAJE_3[INDEX_PEAJE] ) ;
               Serial.println( CICLO_PEAJE_3[INDEX_PEAJE] );
               INDEX_PEAJE = INDEX_PEAJE + 1 ;
            }else{
               CICLOS_PEAJE = 0 ;
               INDEX_PEAJE = 0;
            }         
            break;      
         default:
            break;
      }
   }   
}
