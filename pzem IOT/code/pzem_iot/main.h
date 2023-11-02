#ifndef MAIN_H
#define MAIN_H

#include<WiFi.h>
#include <FirebaseESP32.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <stdio.h> 


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

#define TB1         13
#define READ_TB1    digitalRead(TB1)

#define TB2         12
#define READ_TB2    digitalRead(TB2)

#define TB3         14
#define READ_TB3    digitalRead(TB3)

#define TB4         27
#define READ_TB4    digitalRead(TB4)

#define UP          26
#define READ_UP    digitalRead(UP)

#define DOWN       25
#define READ_DOWN  digitalRead(DOWN)

#define DO         35
#define READ_DO    digitalRead(DO)

#define AO         34
#define READ_AO    analogRead(AO)

/********** khoi tao **************/
FirebaseData firebaseData;
WiFiClient client;
String  path = "/";
FirebaseJson json;
LiquidCrystal_I2C lcd(0x3F,16,2);























#endif
