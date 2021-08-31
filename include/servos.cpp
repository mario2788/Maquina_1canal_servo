#include <Arduino.h>
#include <Servo.h>

// Servos:
Servo SERVO_PEAJE ;
Servo SERVO_CLASIFI ;
Servo SERVO_EXPUL ;

// Ciclos para el motor de PEAJE:
const int CICLO_PEAJE_1 [] = {1500, 1500, 1500, 1501, 1502, 1503, 1505, 1507, 1510, 1512, 1515, 1518, 1522, 1526, 1530, 1534, 1538, 1543, 1548, 1554, 1559, 1565, 1571, 1577, 1583, 1590, 1597, 1604, 1611, 1618, 1625, 1632, 1640, 1647, 1655, 1663, 1671, 1678, 1686, 1694, 1694, 1702, 1710, 1717, 1725, 1733, 1740, 1748, 1756, 1763, 1770, 1777, 1784, 1791, 1798, 1804, 1811, 1817, 1823, 1829, 1834, 1839, 1845, 1849, 1854, 1858, 1862, 1866, 1870, 1873, 1876, 1878, 1881, 1883, 1884, 1886, 1887, 1888, 1888, 1888} ;
const int CICLO_PEAJE_2 [] = {1500, 1500, 1500, 1501, 1502, 1503, 1505, 1507, 1510, 1512, 1515, 1518, 1522, 1526, 1530, 1534, 1538, 1543, 1548, 1554, 1559, 1565, 1571, 1577, 1583, 1590, 1597, 1604, 1611, 1618, 1625, 1632, 1640, 1647, 1655, 1663, 1671, 1678, 1686, 1694, 1694, 1702, 1710, 1717, 1725, 1733, 1740, 1748, 1756, 1763, 1770, 1777, 1784, 1791, 1798, 1804, 1811, 1817, 1823, 1829, 1834, 1839, 1845, 1849, 1854, 1858, 1862, 1866, 1870, 1873, 1876, 1878, 1881, 1883, 1884, 1886, 1887, 1888, 1888, 1888} ;
const int CICLO_PEAJE_3 [] = {1055, 1055, 1056, 1057, 1058, 1060, 1062, 1064, 1066, 1069, 1073, 1077, 1081, 1085, 1089, 1094, 1100, 1105, 1111, 1117, 1123, 1130, 1137, 1144, 1151, 1159, 1166, 1174, 1182, 1190, 1198, 1207, 1215, 1224, 1233, 1242, 1250, 1259, 1268, 1277, 1277, 1286, 1295, 1304, 1313, 1322, 1330, 1339, 1348, 1356, 1364, 1373, 1381, 1388, 1396, 1404, 1411, 1418, 1425, 1431, 1438, 1444, 1449, 1455, 1460, 1465, 1470, 1474, 1478, 1482, 1485, 1488, 1491, 1493, 1495, 1497, 1498, 1499, 1499, 1500} ;
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
const int POS_PEAJE_ON = 1500 ; //90 ;
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