#include <Arduino.h>
#include <Servo.h>

// Servos:
Servo SERVO_PEAJE ;
Servo SERVO_CLASIFI ;
Servo SERVO_EXPUL ;

// Ciclos para el motor de PEAJE:
const int CICLO_PEAJE_1 [] = {500, 502, 511, 525, 545, 570, 601, 637, 677, 722, 771, 825, 881, 941, 1004, 1068, 1135, 1203, 1272, 1342, 1342, 1412, 1481, 1549, 1616, 1680, 1743, 1803, 1859, 1913, 1962, 2007, 2047, 2083, 2114, 2139, 2159, 2173, 2182, 2185} ;
const int CICLO_PEAJE_2 [] = {2185, 2183, 2179, 2172, 2162, 2149, 2134, 2116, 2096, 2073, 2049, 2022, 1994, 1964, 1932, 1900, 1867, 1833, 1798, 1763, 1763, 1728, 1694, 1660, 1626, 1594, 1563, 1533, 1505, 1478, 1453, 1431, 1411, 1393, 1377, 1365, 1355, 1348, 1343, 1342} ;
const int CICLO_PEAJE_3 [] = {1342, 1341, 1336, 1329, 1319, 1307, 1291, 1273, 1253, 1231, 1206, 1179, 1151, 1121, 1090, 1058, 1024, 990, 956, 921, 921, 886, 851, 817, 784, 752, 720, 690, 662, 635, 611, 588, 568, 550, 535, 522, 512, 505, 501, 499} ;

// Ciclos para el motor de CLASIFICADOR:
const int CICLO_CLASIFI_1 [] = {500, 502, 511, 525, 545, 570, 601, 637, 677, 722, 771, 825, 881, 941, 1004, 1068, 1135, 1203, 1272, 1342, 1342, 1412, 1481, 1549, 1616, 1680, 1743, 1803, 1859, 1913, 1962, 2007, 2047, 2083, 2114, 2139, 2159, 2173, 2182, 2185};
const int CICLO_CLASIFI_2 [] = {2185, 2183, 2179, 2172, 2162, 2149, 2134, 2116, 2096, 2073, 2049, 2022, 1994, 1964, 1932, 1900, 1867, 1833, 1798, 1763, 1763, 1728, 1694, 1660, 1626, 1594, 1563, 1533, 1505, 1478, 1453, 1431, 1411, 1393, 1377, 1365, 1355, 1348, 1343, 1342} ;
const int CICLO_CLASIFI_3 [] = {1342, 1341, 1336, 1329, 1319, 1307, 1291, 1273, 1253, 1231, 1206, 1179, 1151, 1121, 1090, 1058, 1024, 990, 956, 921, 921, 886, 851, 817, 784, 752, 720, 690, 662, 635, 611, 588, 568, 550, 535, 522, 512, 505, 501, 499} ;

// Ciclos para el motor de MICROMOTOR:
const int CICLO_EXPUL_1 [] = {500, 502, 511, 525, 545, 570, 601, 637, 677, 722, 771, 825, 881, 941, 1004, 1068, 1135, 1203, 1272, 1342, 1342, 1412, 1481, 1549, 1616, 1680, 1743, 1803, 1859, 1913, 1962, 2007, 2047, 2083, 2114, 2139, 2159, 2173, 2182, 2185} ;
const int CICLO_EXPUL_2 [] = {2185, 2183, 2179, 2172, 2162, 2149, 2134, 2116, 2096, 2073, 2049, 2022, 1994, 1964, 1932, 1900, 1867, 1833, 1798, 1763, 1763, 1728, 1694, 1660, 1626, 1594, 1563, 1533, 1505, 1478, 1453, 1431, 1411, 1393, 1377, 1365, 1355, 1348, 1343, 1342} ;
const int CICLO_EXPUL_3 [] = {1342, 1341, 1336, 1329, 1319, 1307, 1291, 1273, 1253, 1231, 1206, 1179, 1151, 1121, 1090, 1058, 1024, 990, 956, 921, 921, 886, 851, 817, 784, 752, 720, 690, 662, 635, 611, 588, 568, 550, 535, 522, 512, 505, 501, 499} ;


// Dimensiones:
const int SIZE_CICLO_PEAJE_1 = sizeof( CICLO_PEAJE_1 )/2 ;
const int SIZE_CICLO_PEAJE_2 = sizeof( CICLO_PEAJE_2 )/2 ;
const int SIZE_CICLO_PEAJE_3 = sizeof( CICLO_PEAJE_3 )/2 ;

const int SIZE_CICLO_CLASIFI_1 = sizeof( CICLO_CLASIFI_1 )/2 ;
const int SIZE_CICLO_CLASIFI_2 = sizeof( CICLO_CLASIFI_2 )/2 ;
const int SIZE_CICLO_CLASIFI_3 = sizeof( CICLO_CLASIFI_3 )/2 ;

const int SIZE_CICLO_EXPUL_1 = sizeof( CICLO_EXPUL_1 )/2 ;
const int SIZE_CICLO_EXPUL_2 = sizeof( CICLO_EXPUL_2 )/2 ;
const int SIZE_CICLO_EXPUL_3 = sizeof( CICLO_EXPUL_3 )/2 ;



// Posiciones de los motores:
// Al prender:
const int POS_PEAJE_ON = 90 ;
const int POS_CLASIFI_ON =  120 ;
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
// posicion de los servos en el test
int pos = 1200;
void test_servo(){
   
   if( Serial.available() > 0 ){
      // read the incoming byte:
      char received = Serial.read();
      Serial.print( received );
      if( received == 'u' && pos+5 <= 3000){
         pos = pos + 5 ;
         SERVO_PEAJE.writeMicroseconds(pos);
         SERVO_CLASIFI.writeMicroseconds(pos);
         SERVO_EXPUL.writeMicroseconds(pos);
         Serial.println(pos, DEC);
      }

      if( received == 'd' && pos-5 >= 100){
         pos = pos - 5 ;
         SERVO_PEAJE.writeMicroseconds(pos);
         SERVO_CLASIFI.writeMicroseconds(pos);
         SERVO_EXPUL.writeMicroseconds(pos);
         Serial.println(pos, DEC );
      }
      received = ' ';
      Serial.flush();
   }
}