/*                                ______________
                        Reset   -|              |-   Analog_5
      RX: PC/WIFI       Pin_0   -|      A       |-   Analog_4
      TX: PC/WIFI       Pin_1   -|      T       |-   Analog_3
      LED1              Pin_2   -|      M       |-   Analog_2
      LED2              Pin_3   -|      E       |-   Analog_1
      LED3              Pin_4   -|      G       |-   Analog_0
                        VCC     -|      A       |-   GND
                        GND     -|      3       |-   AREF
                        CR_16MHz-|      2       |-   AVCC
                        CR_16MHz-|      8       |-   Pin_13
      BTN1              Pin_5   -|      A       |-   Pin_12
      BTN2              Pin_6   -|              |-   Pin_11      STP1_B2
      BTN3              Pin_7   -|              |-   Pin_10      STP1_B1
      STP1_A1           Pin_8   -|______________|-   Pin_9       STP1_A2

*/

#include <Arduino.h>
#include <AccelStepper.h>

// Definiciones y asignaciones:
// LED y botones
int LED1 = 2 ;
int LED2 = 3 ;
int LED3 = 4 ;
#define BTN1  5
#define BTN2  6
#define BTN3  7

// Definiciones y asignaciones:
//Motores Stepper
AccelStepper stepper1(AccelStepper::FULL4WIRE, 8, 9, 10, 11);

void setup() {
   // put your setup code here, to run once:
   // leds:
   pinMode( LED1, OUTPUT );
   pinMode( LED2, OUTPUT );
   pinMode( LED3, OUTPUT );
   // botones:
   pinMode( BTN1, INPUT ) ;
   pinMode( BTN2, INPUT ) ;
   pinMode( BTN3, INPUT ) ;
   // motores stepper:
   stepper1.setMaxSpeed(300);
   stepper1.setAcceleration(20);
   stepper1.moveTo(400) ;

}
// funciones:
int readBtn( int btn, int stateActive);

void loop() {
   // put your main code here, to run repeatedly:
   if (stepper1.distanceToGo() == 0){
      stepper1.moveTo(-stepper1.currentPosition());
   }
   stepper1.run();

   if( readBtn( BTN1, HIGH ) == BTN1 ){
      digitalWrite( LED1, HIGH );
      delay(100);
   }else{
      digitalWrite( LED1, LOW );
   }
   if( readBtn( BTN2, HIGH ) == BTN2 ){
      digitalWrite( LED2, HIGH );
      delay(100);
   }else{
      digitalWrite( LED2, LOW );
   }
   if( readBtn( BTN3, HIGH ) == BTN3 ){
      digitalWrite( LED3, HIGH );
      delay(100);
   }else{
      digitalWrite( LED3, LOW );
   }

}

// Rutina para leer los botones, según el botón con el estado
// en coincidencia con stateActive, se prende el LED correspodiente.
// La funcion retorna el valor del boton que fue oprimido una vez
// este es liberado por el usuario.
int readBtn( int btn, int stateActive){

   int valueReturn = 0 ;
   int valueBTN = digitalRead( btn );

   while( valueBTN == stateActive ){
      valueBTN = digitalRead( btn ) ;

      switch ( btn ) {
         case BTN1:
            digitalWrite( LED1, HIGH );
            valueReturn = BTN1 ;
            break ;
         case BTN2:
            digitalWrite( LED2, HIGH );
            valueReturn = BTN2 ;
            break ;
         case BTN3:
            digitalWrite( LED3, HIGH );
            valueReturn = BTN3 ;
            break ;
         default:
            digitalWrite( LED1, LOW );
            digitalWrite( LED2, LOW );
            digitalWrite( LED3, LOW );
            break ;
      }
   }
   return valueReturn ;
}
