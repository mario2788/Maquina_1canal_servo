#include <Arduino.h>


   // LED y botones
   #define LED_AZUL   2
   #define LED_VERDE  3
   #define LED_ROJO   4
   #define BTN_AZUL  8
   #define BTN_VERDE 6
   #define BTN_ROJO  7

   
   // Rutina para leer los botones, según el botón con el estado
   // en coincidencia con stateActive se prende el LED correspodiente.
   // La funcion retorna el valor del boton que fue oprimido una vez
   // este es liberado por el usuario.
   boolean readBtn( int btn, int stateActive){

      boolean valueReturn = false ;
      int valueBTN = digitalRead( btn );

      if ( valueBTN == stateActive ) {

         switch ( btn ) {
            case BTN_AZUL:
               digitalWrite( LED_AZUL, HIGH );
               valueReturn = true ;
               Serial.write("Boton azul oprimido");
               break ;
            case BTN_VERDE:
               digitalWrite( LED_VERDE, HIGH );
               valueReturn = true ;
               Serial.write("Boton verde oprimido");
               break ;
            case BTN_ROJO:
               digitalWrite( LED_ROJO, HIGH );
               valueReturn = true ;
               Serial.write("Boton rojo oprimido");
               break ;
            default:
               valueReturn = 0 ;
               break ;
         }


         while( valueBTN == stateActive ){
            valueBTN = digitalRead( btn ) ;
            delay( 10 ) ;
         }
      }

      return valueReturn ;
   }

void test_leds_btns(){
   // test led y botones
   if( readBtn( BTN_AZUL, HIGH )  ){
      digitalWrite( LED_AZUL, LOW );
      Serial.println("Led azul");
   }

   if( readBtn( BTN_VERDE, HIGH )  ){
      digitalWrite( LED_VERDE, LOW );
      Serial.println("Led verde");
   }

   if( readBtn( BTN_ROJO, HIGH )  ){
      digitalWrite( LED_ROJO, LOW );
      Serial.println("Led rojo");
   }

}
