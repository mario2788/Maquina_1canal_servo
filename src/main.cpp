/*                                 ______________
                        Reset   -|1             28|-   Analog_5
      RX: PC/WIFI       Pin_0   -|2      A      27|-   Analog_4
      TX: PC/WIFI       Pin_1   -|3      T      26|-   Analog_3
      LED_AZUL          Pin_2   -|4      M      25|-   Analog_2
      LED_VERDE         Pin_3   -|5      E      24|-   Analog_1
      LED_ROJO          Pin_4   -|6      G      23|-   Analog_0
                        VCC     -|7      A      22|-   GND
                        GND     -|8      3      21|-   AREF
                        CR_16MHz-|9      2      20|-   AVCC
                        CR_16MHz-|10     8      19|-   Pin_13      SCK
      SERVO_PEAJE       Pin_5   -|11     A      18|-   Pin_12      MISO
      BTN-VERDE         Pin_6   -|12            17|-   Pin_11      MOSI
      BTN_ROJO          Pin_7   -|13            16|-   Pin_10      SERVO_EXPULSION
      BTN_AZUL          Pin_8   -|14____________15|-   Pin_9       SERVO_CLASIFICADOR

   Motor mecanismo de admisión huevo a huevo ->    motor peaje
	Motor mecanismo de rotación vertical      ->    motor clasificador
	Motor mecanismo de expulsión de huevo     ->    micromotor

*/
// dar privilegios al conversor Serial
// sudo chmod ugo+x+r+w /dev/ttyUSB0
#include <Arduino.h>
#include <Servo.h>

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

boolean servo_run() ;
unsigned char categoria(float value);
void timerTwoSetup();
boolean value = false;
unsigned int countTimerTwo = 0;
unsigned long t = 0;


void setup() {

   // leds:
   pinMode( LED_AZUL,   OUTPUT );
   pinMode( LED_VERDE,  OUTPUT );
   pinMode( LED_ROJO,   OUTPUT );
   digitalWrite( LED_AZUL, HIGH ) ;
   digitalWrite( LED_VERDE, HIGH ) ;
   digitalWrite( LED_ROJO, HIGH ) ;

   // botones:
   pinMode( BTN_AZUL,   INPUT ) ;
   pinMode( BTN_VERDE,  INPUT ) ;
   pinMode( BTN_ROJO,   INPUT ) ;

  
   // Serial:
   Serial.begin(115200);
   Serial.println("FirmWare With Servo motor");

   // Servos:
   SERVO_PEAJE.attach   ( 5, 500, 2500 ) ; // ( pin, min_ms, max_ms)
   SERVO_CLASIFI.attach ( 9, 500, 2500 ) ;
   SERVO_EXPUL.attach   ( 10, 500, 2500) ;
   SERVO_PEAJE.write( POS_PEAJE_ON ) ;
   SERVO_CLASIFI.write( POS_CLASIFI_ON ) ;
   SERVO_EXPUL.write ( POS_EXPUL_ON ) ;
   
   // Celda de carga:
   pinMode( LOADCELL_SCK_PIN, OUTPUT );
   pinMode( LOADCELL_DOUT_PIN, INPUT );

   // Iniciar sensor
   bascula.begin( LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN );
   //calibracion de la celda
   calibrarCelda() ;
   
   //variables:
   digitalWrite( LED_AZUL, HIGH ) ;
   digitalWrite( LED_VERDE, HIGH ) ;
   digitalWrite( LED_ROJO, HIGH ) ;


   // Configuración de TIMER2
   timerTwoSetup();
  
}

void loop() {
   // test_leds_btns() ;
   // test_celda();
   // test_servo();
   

   // Rutina principal
   float value_celda = 0 ;
   
//    Espera hasta que se oprima el boton verde
   Serial.println(" ");
   Serial.println("En espera del boton verde");
   Serial.println("En espera...");
   while( 1 ){
      if( readBtn( BTN_VERDE, HIGH ) ){
          break ;
      }
   }

   //Inicio de la rutina de trabajo:
//    unsigned char cat = 0 ;

   // carga huevo a la celda:
   Serial.println("Carga del ciclo 1") ;
   CICLOS_PEAJE = 1 ; //Se da comienzo al movimiento.
   CICLOS_EXPUL = 1 ;
   CICLOS_CLASIFI = 1 ;
   while( ( CICLOS_PEAJE || CICLOS_EXPUL || CICLOS_CLASIFI ) == true ){
        // bascula.tare();
        Serial.println( bascula.get_units(1) ,2 ) ;
   }
    // bascula.get_units(2);

   // carga huevo a la celda:
   Serial.println("Carga del ciclo 2") ;
   CICLOS_PEAJE = 2 ; //Se da comienzo al movimiento.
   CICLOS_EXPUL = 2 ;
   CICLOS_CLASIFI = 2 ;
   while( ( CICLOS_PEAJE || CICLOS_EXPUL || CICLOS_CLASIFI ) == true ){
    //   bascula.tare();
      Serial.println( bascula.get_units(1) ,2 ) ;
   } 

   // carga huevo a la celda:
   Serial.println("Carga del ciclo 3") ;
   CICLOS_PEAJE = 3 ; //Se da comienzo al movimiento.
   CICLOS_EXPUL = 3 ;
   CICLOS_CLASIFI = 3 ;
   while( ( CICLOS_PEAJE || CICLOS_EXPUL || CICLOS_CLASIFI ) == true ){
    //   bascula.tare();
      Serial.println( bascula.get_units(1) ,2 ) ;
   } 


   
}

ISR(TIMER2_COMPA_vect){
   countTimerTwo = countTimerTwo + 1 ;
   if( countTimerTwo == 2 ){      
      countTimerTwo = 0 ;
    //   Serial.print("Time Inter:");
    //   Serial.println(millis()-t);
      t = millis();
      value = !value;
      digitalWrite( LED_VERDE, value ) ;
      servo_run();
    }
}

void timerTwoSetup(){
  // Configuración de TIMER2
  TCCR2A = 0;                // El registro de control A queda todo en 0
  TCCR2B = 0;                //limpia registrador
  TCNT2  = 0;                //Inicializa el temporizador
  OCR2A = 0xF9;            // carga el registrador de comparación: 16MHz/1024/62.5Hz -1 = 249 = 0xF9
  TCCR2B |= (1 << WGM22)|(1<<CS22)|(1 << CS21)|(1 << CS20);   // modo CTC: WGM12, prescaler de 1024: CS22 = 1, CS21 = 1 y CS20 = 1 
  TIMSK2 |= (1 << OCIE2A);  // habilita interrupción por igualdad de comparación
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

boolean servo_run(){ 
   // administracion de ciclos del motor clasificador
   boolean flag_mov = false ;
//    Serial.println(" ") ;

   if( CICLOS_CLASIFI != 0 ){
      flag_mov = true ;
    //   Serial.print("clasificador ");
      switch ( CICLOS_CLASIFI ){
         case 1:
            if( INDEX_CLASIFI <  SIZE_CICLO_CLASIFI_1-1 ){
               SERVO_CLASIFI.writeMicroseconds( CICLO_CLASIFI_1[INDEX_CLASIFI] ) ;
               INDEX_CLASIFI = INDEX_CLASIFI + 1 ;
            }else{
               CICLOS_CLASIFI = 0 ;
               INDEX_CLASIFI = 0;
            }
            break;
         case 2:
            if( INDEX_CLASIFI <  SIZE_CICLO_CLASIFI_2 - 1 ){
               SERVO_CLASIFI.writeMicroseconds( CICLO_CLASIFI_2[INDEX_CLASIFI] ) ;
               INDEX_CLASIFI = INDEX_CLASIFI + 1 ;
            }else{
               CICLOS_CLASIFI = 0 ;
               INDEX_CLASIFI = 0;
            }
            break; 
         case 3:
            if( INDEX_CLASIFI <  SIZE_CICLO_CLASIFI_3 - 1 ){
               SERVO_CLASIFI.writeMicroseconds( CICLO_CLASIFI_3[INDEX_CLASIFI] ) ;
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
      flag_mov = true ;
    //   Serial.print("expulsor ");
      switch ( CICLOS_EXPUL ){
         case 1:
            if( INDEX_EXPUL <  SIZE_CICLO_EXPUL_1 - 1 ){
               INDEX_EXPUL = INDEX_EXPUL + 1 ;
               SERVO_EXPUL.writeMicroseconds( CICLO_EXPUL_1[INDEX_EXPUL] ) ;
            }else{
               CICLOS_EXPUL = 0 ;
               INDEX_EXPUL = 0;
            }
            break;
         case 2:
            if( INDEX_EXPUL <  SIZE_CICLO_EXPUL_2 - 1 ){
               INDEX_EXPUL = INDEX_EXPUL + 1 ;
               SERVO_EXPUL.writeMicroseconds( CICLO_EXPUL_2[INDEX_EXPUL] ) ;
            }else{
               CICLOS_EXPUL = 0 ;
               INDEX_EXPUL = 0;
            }
            break; 
         case 3:
            if( INDEX_EXPUL <  SIZE_CICLO_EXPUL_3 - 1 ){
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

  // administracion de ciclos del motor de peaje
   if( CICLOS_PEAJE !=0 ){
      flag_mov = true ;
    //   Serial.print("peaje ");
      switch ( CICLOS_PEAJE ){
         case 1:
            if( INDEX_PEAJE < SIZE_CICLO_PEAJE_1 - 1 ){               
               SERVO_PEAJE.writeMicroseconds( CICLO_PEAJE_1[INDEX_PEAJE] ) ;
               INDEX_PEAJE = INDEX_PEAJE + 1 ;
            }else{
               CICLOS_PEAJE = 0 ;
               INDEX_PEAJE = 0;
            }
            break;
         case 2:
            if( INDEX_PEAJE <  SIZE_CICLO_PEAJE_2 - 1 ){
               SERVO_PEAJE.writeMicroseconds( CICLO_PEAJE_2[INDEX_PEAJE] ) ;
               INDEX_PEAJE = INDEX_PEAJE + 1 ;
            }else{
               CICLOS_PEAJE = 0 ;
               INDEX_PEAJE = 0;
            }
            break; 
         case 3:
            if( INDEX_PEAJE <  SIZE_CICLO_PEAJE_3 - 1 ){
               SERVO_PEAJE.writeMicroseconds( CICLO_PEAJE_3[INDEX_PEAJE] ) ;               
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

   return flag_mov ; 
}