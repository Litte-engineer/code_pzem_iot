#ifndef MAIN_H
#define MAIN_H

#include<WiFi.h>
#include <FirebaseESP32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include <EEPROM.h>
#include <PZEM004Tv30.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

#define SIM_TX 18
#define SIM_RX 19
SoftwareSerial sim;

#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17
#define PZEM_SERIAL Serial2
PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);

#define ADD_VOLT        0
#define ADD_CURRENT     10
#define ADD_GAS        20
#define ADD_MUM         30

#define     MONEY_LEVEL_1      1678
#define     MONEY_LEVEL_2     1734
#define     MONEY_LEVEL_3     2014
#define     MONEY_LEVEL_4      2536
#define     MONEY_LEVEL_5      2834
#define     MONEY_LEVEL_6      2927

#define    ENERGY_LEVEL_1      50
#define    ENERGY_LEVEL_2      100
#define    ENERGY_LEVEL_3      200
#define    ENERGY_LEVEL_4      300
#define    ENERGY_LEVEL_5      400

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
LiquidCrystal_I2C lcd(0x27, 20, 4);




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


void sim_at_wait()
{
  delay(200);
  if (sim.available())
  {
    Serial.println(sim.readString());
  }
}
bool sim_at_cmd(String cmd)
{
  sim.println(cmd);
  delay(500);
  //sim_at_wait();
}
bool sim_at_send(char c) {
  sim.write(c);
}


void sent_sms(String mumber, String mess)
{
  String num = (String)mumber[1] + (String)mumber[2] + (String)mumber[3]
               + (String)mumber[4] + (String)mumber[5] + (String)mumber[6] + (String)mumber[7]
               + (String)mumber[8] + (String)mumber[9];
  Serial.println(num);
  String com = "+84" + (String)num;
  Serial.println(com);
  sim_at_cmd("AT+CMGF=1");
  String temp = "AT+CMGS=\"";
  temp += (String)com;
  temp += "\"";
  sim_at_cmd(temp);
  sim_at_cmd(mess);
  sim_at_send(0x1A);
  delay(3000);
}

void sim_init(void)
{
  sim_at_cmd("AT");
  sim_at_cmd("ATI");
  sim_at_cmd("AT+CPIN?");
  sim_at_cmd("AT+CSQ");
  sim_at_cmd("AT+CIMI");
}


















#endif
