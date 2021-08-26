#include <Arduino.h>
#include <Servo.h>

// Servos:
Servo SERVO_PEAJE ;
Servo SERVO_CLASIFI ;
Servo SERVO_EXPUL ;

// Ciclos para el motor de PEAJE:
const int CICLO_PEAJE_1 [] = {1222, 1224, 1229, 1239, 1251, 1268, 1287, 1309, 1333, 1359, 1386, 1415, 1444, 1444, 1473, 1501, 1529, 1555, 1579, 1601, 1620, 1636, 1649, 1659, 1664, 1666} ;
const int CICLO_PEAJE_2 [] = {1222, 1224, 1229, 1239, 1251, 1268, 1287, 1309, 1333, 1359, 1386, 1415, 1444, 1444, 1473, 1501, 1529, 1555, 1579, 1601, 1620, 1636, 1649, 1659, 1664, 1666} ;
const int CICLO_PEAJE_3 [] = {1222, 1224, 1229, 1239, 1251, 1268, 1287, 1309, 1333, 1359, 1386, 1415, 1444, 1444, 1473, 1501, 1529, 1555, 1579, 1601, 1620, 1636, 1649, 1659, 1664, 1666} ;
// Ciclos para el motor de CLASIFICADOR:
const int CICLO_CLASIFI_1 [] = {1222, 1224, 1229, 1239, 1251, 1268, 1287, 1309, 1333, 1359, 1386, 1415, 1444, 1444, 1473, 1501, 1529, 1555, 1579, 1601, 1620, 1636, 1649, 1659, 1664, 1666} ;
const int CICLO_CLASIFI_2 [] = {1222, 1224, 1229, 1239, 1251, 1268, 1287, 1309, 1333, 1359, 1386, 1415, 1444, 1444, 1473, 1501, 1529, 1555, 1579, 1601, 1620, 1636, 1649, 1659, 1664, 1666} ;
const int CICLO_CLASIFI_3 [] = {1222, 1224, 1229, 1239, 1251, 1268, 1287, 1309, 1333, 1359, 1386, 1415, 1444, 1444, 1473, 1501, 1529, 1555, 1579, 1601, 1620, 1636, 1649, 1659, 1664, 1666} ;
// Ciclos para el motor de MICROMOTOR:
const int CICLO_EXPUL_1 [] = {1222, 1224, 1229, 1239, 1251, 1268, 1287, 1309, 1333, 1359, 1386, 1415, 1444, 1444, 1473, 1501, 1529, 1555, 1579, 1601, 1620, 1636, 1649, 1659, 1664, 1666} ;
const int CICLO_EXPUL_2 [] = {1222, 1224, 1229, 1239, 1251, 1268, 1287, 1309, 1333, 1359, 1386, 1415, 1444, 1444, 1473, 1501, 1529, 1555, 1579, 1601, 1620, 1636, 1649, 1659, 1664, 1666} ;
const int CICLO_EXPUL_3 [] = {1222, 1224, 1229, 1239, 1251, 1268, 1287, 1309, 1333, 1359, 1386, 1415, 1444, 1444, 1473, 1501, 1529, 1555, 1579, 1601, 1620, 1636, 1649, 1659, 1664, 1666} ;


// Posiciones de los motores:
// Al prender:
const int POS_PEAJE_ON = 90 ;
const int POS_CLASIFI_ON = 120 ;
const int POS_EXPUL_ON = 140 ;
// Tras el oprimir el boton verde
const int POS_PEAJE_LED_VERDE = 90 ;
const int POS_CLASIFI_LED_VERDE = 120 ;
const int POS_EXPUL_LED_VERDE = 140 ;

// Ciclos: {1,2,3,4...}:
unsigned char CICLOS_EXPUL = 0 ;
unsigned char CICLOS_CLASIFI = 0 ; 
unsigned char CICLOS_PEAJE = 0 ;
// indexadores: {1,2,3...,255 }
unsigned char INDEX_EXPUL = 0 ;
unsigned char INDEX_CLASIFI = 0 ; 
unsigned char INDEX_PEAJE = 0 ;

// test servo motores
void test_servo(){
   // posicion de los servos en el test
   int pos = 0;
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
}