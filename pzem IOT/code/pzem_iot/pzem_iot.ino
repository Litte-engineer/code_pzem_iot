#include "main.h"
const char* ssid = "KS Chuong"; //Enter SSID
const char* password = "0946627518"; //Enter Password

/****** khai bao bien *****/

uint32_t time_millis;
uint32_t time_tran;

typedef struct
{
  bool tb1;
  bool tb2;
  bool tb3;
  bool tb4;

  uint8_t old_tb1;
  uint8_t old_tb2;
  uint8_t old_tb3;
  uint8_t old_tb4;

  uint8_t bnt_tb1;
  uint8_t bnt_tb2;
  uint8_t bnt_tb3;
  uint8_t bnt_tb4;
  uint8_t bnt_up;
  uint8_t bnt_down;

  uint8_t transmit;

  uint8_t mode;
} Status;
Status status;

typedef struct
{
  float vol ;
  float current ;
  float power ;
  float gas;
} Data;
Data data;

typedef struct
{
  uint16_t time;
  uint8_t tb1;
  uint8_t tb2;
  uint8_t tb3;
  uint8_t tb4;

  uint8_t tran;
  uint8_t old_tran;

} Count;
Count count;

/***** khai bao ham nguyen mau *****/
void read_firebase(void);
void control(void);
void read_bnt(void);
void run_mode(void);
void setting_mode(void);
void run_display(void);
void setting_display(void);




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("*");
  }

  Serial.println("");
  Serial.println("WiFi connection Successful");
  Serial.print("The IP Address of ESP32 Module is: ");
  Serial.print(WiFi.localIP());// Print the IP address

  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(RL1, OUTPUT);
  pinMode(RL2, OUTPUT);
  pinMode(RL3, OUTPUT);
  pinMode(RL4, OUTPUT);

  pinMode(TB1, INPUT_PULLUP);
  pinMode(TB2, INPUT_PULLUP);
  pinMode(TB3, INPUT_PULLUP);
  pinMode(TB4, INPUT_PULLUP);
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(DO, INPUT_PULLUP);


  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  // put your main code here, to run repeatedly:
  read_bnt();
  if (status.mode == 0)
  {
    control();
    run_mode();
    //if (status.transmit == 1)
    //{
    read_firebase();
    //status.transmit = 0;
    //}
  }
  else
  {
    setting_mode();
  }
}


/**** ham doc du lieu firebase ****/
void read_firebase(void)
{

  if (Firebase.getString(firebaseData, path + "/TB1"))
  {
    String data_tb1 = firebaseData.stringData();
    Serial.print("thiet bi 1 :");
    Serial.print(data_tb1[3]);
    if (data_tb1[3] == 'N') status.tb1 = 1;
    else if (data_tb1[3] == 'F') status.tb1 = 0;
    Serial.print(status.bnt_tb1);
    Serial.println(status.tb1);
  }
  if (Firebase.getString(firebaseData, path + "/TB2"))
  {
    Serial.print("thiet bi 2 :");
    String data_tb2 = firebaseData.stringData();
    Serial.print(data_tb2[3]);
    if (data_tb2[3] == 'N')
    {
      status.tb2 = 1;
    }
    else status.tb2 = 0;
    Serial.print(status.bnt_tb2);
    Serial.println(status.tb2);
  }
  if (Firebase.getString(firebaseData, path + "/TB3"))
  {
    Serial.print("thiet bi 3 :");
    String data_tb3 = firebaseData.stringData();
    Serial.print(data_tb3[3]);
    if (data_tb3[3] == 'N')
    {
      status.tb3 = 1;
    }
    else status.tb3 = 0;
    Serial.print(status.bnt_tb3);
    Serial.println(status.tb3);
  }
  if (Firebase.getString(firebaseData, path + "/TB4"))
  {
    Serial.print("thiet bi 4 :");
    String data_tb4 = firebaseData.stringData();
    Serial.print(data_tb4[3]);
    if (data_tb4[3] == 'N')
    {
      status.tb4 = 1;
    }
    else status.tb4 = 0;
    Serial.print(status.bnt_tb4);
    Serial.println(status.tb4);
  }

  /**** gui dien dap ****/
  char vol[7];
  sprintf(vol, "%0.2f", data.vol);
  Firebase.setString(firebaseData, path + "/vol", (String)vol);

  /**** gui dong dien *****/
  char current[7];
  sprintf(current, "%0.2f", data.current);
  Firebase.setString(firebaseData, path + "/current", (String)current);

  /*** gui cong suat ****/
  char power[7];
  sprintf(power, "%0.2f", data.power);
  Firebase.setString(firebaseData, path + "/power", (String)power);
  delay(50);

}
/************ ham dieu khien thiet bi **************/
void control(void)
{
  if (status.tb1 == 1) RL1_ON;
  else RL1_OFF;
  if (status.tb2 == 1) RL2_ON;
  else RL2_OFF;
  if (status.tb3 == 1) RL3_ON;
  else  RL3_OFF;
  if (status.tb4 == 1)  RL4_ON;
  else RL4_OFF;
}

/**************** ham doc nut nhan ******/
void read_bnt(void)
{
  status.bnt_up   = READ_UP;
  status.bnt_down = READ_DOWN;
  status.bnt_tb1  = READ_TB1;
  status.bnt_tb2  = READ_TB2;
  status.bnt_tb3  = READ_TB3;
  status.bnt_tb4  = READ_TB4;

}
/************ che do hoat dong ***************/
void run_mode(void)
{
  /******** chuyen che do cai dat thong so ****/
  if (status.bnt_up == 0)
  {
    if (millis() - time_millis > 1000)
    {
      count.time ++;
      if (count.time > 5) count.time = 6;
      time_millis =  millis();
    }
    if (count.time == 5)
    {
      status.mode = 1;
      lcd.clear();
    }
  }
  else count.time = 0;
  /******** bat tat thiet bi 1 bang nut *************/
  if (status.bnt_tb1 == 0)
  {
    if (count.tb1 == 0)
    {
      status.tb1 = !status.tb1;
      if (status.tb1 == 1) Firebase.setString(firebaseData, path + "/TB1", "ON");
      else Firebase.setString(firebaseData, path + "/TB1", "OFF");
      count.tb1 = 1;
    }
  }
  else count.tb1 = 0;
  /******** bat tat thiet bi 2 bang nut *************/
  if (status.bnt_tb2 == 0)
  {
    if (count.tb2 == 0)
    {
      status.tb2 = !status.tb2;
      if (status.tb2 == 1) Firebase.setString(firebaseData, path + "/TB2", "ON");
      else Firebase.setString(firebaseData, path + "/TB2", "OFF");
      count.tb2 = 1;
    }
  }
  else count.tb2 = 0;
  /******** bat tat thiet bi 3 bang nut *************/
  if (status.bnt_tb3 == 0)
  {
    if (count.tb3 == 0)
    {
      status.tb3 = !status.tb3;
      if (status.tb3 == 1) Firebase.setString(firebaseData, path + "/TB3", "ON");
      else Firebase.setString(firebaseData, path + "/TB3", "OFF");
      count.tb3 = 1;
    }
  }
  else count.tb3 = 0;
  /******** bat tat thiet bi 4 bang nut *************/
  if (status.bnt_tb4 == 0)
  {
    if (count.tb4 == 0)
    {
      status.tb4 = !status.tb4;
      if (status.tb4 == 1) Firebase.setString(firebaseData, path + "/TB4", "ON");
      else Firebase.setString(firebaseData, path + "/TB4", "OFF");
      count.tb4 = 1;
    }
  }
  else count.tb4 = 0;

}
/************** che do cai dat ****************/
void setting_mode(void)
{

}
