#ifndef MAIN_H
#define MAIN_H

#include<WiFi.h>
#include <FirebaseESP32.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <stdio.h> 
#include <EEPROM.h>
#include <PZEM004Tv30.h>

#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17
#define PZEM_SERIAL Serial2
PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);

#define ADD_VOLT        0
#define ADD_CURRENT     10
#define ADD_GAS        20


#define pzemt        Serial2

#define FIREBASE_HOST "https://pzemt-iot-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "JY9IoWAjx649T8XKBYH9jJAinAmfVhbtMxkGbBRN"

#define RL1         32
#define RL2         33
#define RL3         5
#define RL4         23

#define RL1_ON      digitalWrite(RL1, HIGH)
#define RL2_ON      digitalWrite(RL2, HIGH)
#define RL3_ON      digitalWrite(RL3, HIGH)
#define RL4_ON      digitalWrite(RL4, HIGH)
#define RL1_OFF      digitalWrite(RL1, LOW)
#define RL2_OFF      digitalWrite(RL2, LOW)
#define RL3_OFF      digitalWrite(RL3, LOW)
#define RL4_OFF      digitalWrite(RL4, LOW)

#define BUZZER      4

#define BUZZER_ON   digitalWrite(BUZZER, HIGH)
#define BUZZER_OFF  digitalWrite(BUZZER, LOW)

#define TB4         26
#define READ_TB4    digitalRead(TB4)

#define TB2         14
#define READ_TB2    digitalRead(TB2)

#define TB3         12
#define READ_TB3    digitalRead(TB3)

#define TB1         27
#define READ_TB1    digitalRead(TB1)

#define UP          25
#define READ_UP    digitalRead(UP)

//#define DOWN       25
//#define READ_DOWN  digitalRead(DOWN)

#define DO         35
#define READ_DO    digitalRead(DO)

#define AO         34
#define READ_AO    analogRead(AO)

/********** khoi tao **************/
FirebaseData firebaseData;
WiFiClient client;
String  path = "/";
FirebaseJson json;
LiquidCrystal_I2C lcd(0x27,20,4);




/***** luu du lieu tu EEPROM ********/
void EEPROM_put(int add, String Data)
{
  for (int i = 0; i < 10 ; i++)
  {
    EEPROM.write(i + add, Data[i]);
    EEPROM.commit();
  }
}
/******** lay du lieu tu eeprom *********/
String EEPROM_get(int add)
{
  String sdt = "";
  for (int i = add; i < add + 10; i++)
  {
    char c  = EEPROM.read(i);
    sdt += (String)c;
  }
  return sdt;
}


















#endif
