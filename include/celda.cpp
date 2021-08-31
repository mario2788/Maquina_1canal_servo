// Rutina para calibrar la celda según una masa conocida
#include <Arduino.h>
#include <HX711.h>


   // HX711 circuit wiring
   const int LOADCELL_SCK_PIN    = 13 ;
   const int LOADCELL_DOUT_PIN   = 12 ;
   HX711 bascula;
   const int masa_patron = 22 ;

   void calibrarCelda(){
      // How to calibrate your load cell:
      // 1. Call `set_scale()` with no parameter.
      // 2. Call `tare()` with no parameter.
      // 3. Place a known weight on the scale and call `get_units(10)`.
      // 4. Divide the result in step 3 to your known weight. You should
      //    get about the parameter you need to pass to `set_scale()`.
      // 5. Adjust the parameter in step 4 until you get an accurate reading.

      bascula.set_scale();
      Serial.println( "Calibracion: set_scale()" ) ;
      bascula.tare();
      Serial.println( "Calibracion: tare()" );

      unsigned int delay_200m = millis() ;
      boolean stateLed = HIGH ;
      digitalWrite( LED_AZUL, stateLed ) ;
      while( !readBtn( BTN_AZUL, HIGH ) ) {
         if( millis() > (delay_200m + 200) ){
            stateLed = !stateLed;
            digitalWrite( LED_AZUL, stateLed ) ;
            delay_200m = millis() ;
         }
      }
      digitalWrite( LED_AZUL, LOW ) ;


      float value = bascula.get_units(10) ;
      Serial.println( "*****" );
      Serial.print("value:");
      Serial.println( value );

      value = value/masa_patron ;
      Serial.print("value/22:" );
      Serial.println( value );
      bascula.set_scale( value ) ;

      // while( true ){
      //    value = bascula.get_units(10); // Promedio de 10 lecturas
      //    Serial.print("Lectura:");
      //    Serial.println( value );
      //    if( value >= (masa_patron - 2 ) && value <= (masa_patron + 2 )){
      //       break ;
      //    }
      // }
      Serial.println( "Configuracion terminada." );
      digitalWrite( LED_ROJO, HIGH ) ;
      digitalWrite( LED_VERDE, HIGH ) ;
      digitalWrite( LED_AZUL, HIGH ) ;
   }

   void test_celda(){
      float value = bascula.get_units(3); // Tres lecturas
      Serial.print( "Peso:" ) ;
      Serial.println( value ) ;
   }