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

  bool old_tb1;
  bool old_tb2;
  bool old_tb3;
  bool old_tb4;

  bool bnt_tb1;
  bool bnt_tb2;
  bool bnt_tb3;
  bool bnt_tb4;
  bool bnt_up;
  uint8_t bnt_down;

  bool old_bnt_tb1;
  bool old_bnt_tb2;
  bool old_bnt_tb3;
  bool old_bnt_tb4;
  bool old_bnt_up;

  uint8_t transmit;

  uint8_t mode;

  uint8_t update;

  uint8_t cursor;
} Status;
Status status;

typedef struct
{
  float voltage ;
  float current ;
  float energy ;
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

typedef struct
{
  uint16_t volt;
  uint8_t current;
  uint8_t gas;
} Setting;
Setting setting;

/***** khai bao ham nguyen mau *****/
void read_firebase(void);
void control(void);
void read_bnt(void);
void run_mode(void);
void setting_mode(void);
void run_display(void);
void setting_display(void);
void update_status(void);




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  EEPROM.begin(512);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("*");
  }

  Serial.println("");
  Serial.println("WiFi connection Successful");
  Serial.print("The IP Address of ESP32 Module is: ");
  Serial.print(WiFi.localIP());// Print the IP address*/

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
  //pinMode(DOWN, INPUT_PULLUP);
  pinMode(DO, INPUT_PULLUP);


  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  read_bnt();
  update_status();
  Serial.println(status.old_bnt_up);

  String vol = EEPROM_get(ADD_VOLT);
  setting.volt = vol.toInt();

  String cu = EEPROM_get(ADD_CURRENT);
  setting.current = cu.toInt();

  String gas = EEPROM_get(ADD_GAS);
  setting.gas = gas.toInt();


}

void loop() {
  // put your main code here, to run repeatedly:
  read_bnt();

  if (status.mode == 0)
  {
    run_display();
    control();
    run_mode();
    read_firebase();
  }
  else
  {
    setting_display();
    setting_mode();
  }
}


/**** ham doc du lieu firebase ****/
void read_firebase(void)
{
  if (millis() - time_tran > 3000)
  {
    count.tran ++;
    time_tran = millis();
  }
  if (count.tran != count.old_tran)
  {
    if (Firebase.getString(firebaseData, path + "/TB1"))
    {
      String data_tb1 = firebaseData.stringData();
      //Serial.print("thiet bi 1 :");
      //Serial.print(data_tb1[3]);
      if (data_tb1[3] == 'N') status.tb1 = 1;
      else if (data_tb1[3] == 'F') status.tb1 = 0;
      //Serial.print(status.bnt_tb1);
      //Serial.println(status.tb1);
    }
    if (Firebase.getString(firebaseData, path + "/TB2"))
    {
      //Serial.print("thiet bi 2 :");
      String data_tb2 = firebaseData.stringData();
      //Serial.print(data_tb2[3]);
      if (data_tb2[3] == 'N')
      {
        status.tb2 = 1;
      }
      else status.tb2 = 0;
      //Serial.print(status.bnt_tb2);
      //Serial.println(status.tb2);
    }
    if (Firebase.getString(firebaseData, path + "/TB3"))
    {
      //Serial.print("thiet bi 3 :");
      String data_tb3 = firebaseData.stringData();
      //Serial.print(data_tb3[3]);
      if (data_tb3[3] == 'N')
      {
        status.tb3 = 1;
      }
      else status.tb3 = 0;
      //Serial.print(status.bnt_tb3);
      //Serial.println(status.tb3);
    }
    if (Firebase.getString(firebaseData, path + "/TB4"))
    {
      //Serial.print("thiet bi 4 :");
      String data_tb4 = firebaseData.stringData();
      //Serial.print(data_tb4[3]);
      if (data_tb4[3] == 'N')
      {
        status.tb4 = 1;
      }
      else status.tb4 = 0;
      //Serial.print(status.bnt_tb4);
      //Serial.println(status.tb4);
    }

    /**** gui dien dap ****/
    char vol[7];
    sprintf(vol, "%0.2f", data.voltage);
    Firebase.setString(firebaseData, path + "/vol", (String)vol);

    /**** gui dong dien *****/
    char current[7];
    sprintf(current, "%0.2f", data.current);
    Firebase.setString(firebaseData, path + "/current", (String)current);

    /*** gui cong suat ****/
    char power[7];
    sprintf(power, "%0.2f", data.energy);
    Firebase.setString(firebaseData, path + "/power", (String)power);
    
    count.old_tran = count.tran;
  }
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
/********* cap nhat bien ***********/
void update_status(void)
{
  status.old_bnt_up = status.bnt_up;
  status.old_bnt_tb1 = status.bnt_tb1;
  status.old_bnt_tb2 = status.bnt_tb2;
  status.old_bnt_tb3 = status.bnt_tb3;
  status.old_bnt_tb4 = status.bnt_tb4;

}

/**************** ham doc nut nhan ******/
void read_bnt(void)
{
  status.bnt_up   = READ_UP;
  status.bnt_tb1  = READ_TB1;
  status.bnt_tb2  = READ_TB2;
  status.bnt_tb3  = READ_TB3;
  status.bnt_tb4  = READ_TB4;
  //Serial.print("up:");
  //Serial.println(status.bnt_up);
  //Serial.print("old up:");
  //Serial.println(status.old_bnt_up);

  //Serial.print("tb1:");
  //Serial.println(status.bnt_tb1);

  //Serial.print("tb2:");
  //Serial.println(status.bnt_tb2);

  //Serial.print("tb3:");
  //Serial.println(status.bnt_tb3);

}
/************ che do hoat dong ***************/
void run_mode(void)
{
  /******** chuyen che do cai dat thong so ****/
  if (status.update == 0)
  {
    if (status.bnt_up != status.old_bnt_up)
    {
      lcd.clear();
      status.update = 1;
      status.old_bnt_up = status.bnt_up;
      status.mode = 1;
    }
  }
  /******** bat tat thiet bi 1 bang nut *************/
  if (status.bnt_tb1 != status.old_bnt_tb1)
  {
    status.tb1 = !status.tb1;
    if (status.tb1 == 1) Firebase.setString(firebaseData, path + "/TB1", "ON");
    else Firebase.setString(firebaseData, path + "/TB1", "OFF");
    status.old_bnt_tb1 = status.bnt_tb1;
  }
  /******** bat tat thiet bi 2 bang nut *************/
  if (status.bnt_tb2 != status.old_bnt_tb2)
  {
    status.tb2 = !status.tb2;
    if (status.tb2 == 1) Firebase.setString(firebaseData, path + "/TB2", "ON");
    else Firebase.setString(firebaseData, path + "/TB2", "OFF");
    status.old_bnt_tb2 = status.bnt_tb2;
  }
  /******** bat tat thiet bi 3 bang nut *************/
  if (status.bnt_tb3 != status.old_bnt_tb3)
  {
    status.tb3 = !status.tb3;
    if (status.tb3 == 1) Firebase.setString(firebaseData, path + "/TB3", "ON");
    else Firebase.setString(firebaseData, path + "/TB3", "OFF");
    status.old_bnt_tb3 = status.bnt_tb3;
  }
  /******** bat tat thiet bi 4 bang nut *************/
  if (status.bnt_tb4 != status.old_bnt_tb4)
  {
    status.tb4 = !status.tb4;
    if (status.tb4 == 1) Firebase.setString(firebaseData, path + "/TB4", "ON");
    else Firebase.setString(firebaseData, path + "/TB4", "OFF");
    status.old_bnt_tb4 = status.bnt_tb4;
  }

}
/************** che do cai dat ****************/
void setting_mode(void)
{
  if (status.update == 1)
  {
    if (status.bnt_up != status.old_bnt_up)
    {
      lcd.clear();
      status.update = 0;
      status.old_bnt_up = status.bnt_up;

      EEPROM_put(ADD_VOLT, (String)setting.volt);
      EEPROM_put(ADD_CURRENT, (String)setting.current);
      EEPROM_put(ADD_GAS, (String)setting.gas);

      status.mode = 0;
    }
  }

}
/********** man hinh chay ***************/
void run_display(void)
{
  lcd.setCursor(0, 0);
  lcd.print("Dien ap   : ");
  
  data.voltage = pzem.voltage();
  lcd.setCursor(11, 0);
  lcd.print(data.voltage);

  lcd.setCursor(17, 0);
  lcd.print("V");

  lcd.setCursor(0, 1);
  lcd.print("Dong dien : ");
  data.current = pzem.current();
  lcd.setCursor(11, 1);
  lcd.print(data.current);

  lcd.setCursor(17, 1);
  lcd.print("A");

  lcd.setCursor(0, 2);
  lcd.print("So dien   : ");
  data.energy = pzem.energy();
  lcd.setCursor(11, 2);
  lcd.print(data.energy);

  lcd.setCursor(17, 2);
  lcd.print("KWh");

  lcd.setCursor(0, 3);
  if (status.tb1 == 1) lcd.print("ON ");
  else lcd.print("OFF");

  lcd.setCursor(6, 3);
  if (status.tb2 == 1) lcd.print("ON ");
  else lcd.print("OFF");

  lcd.setCursor(11, 3);
  if (status.tb3 == 1) lcd.print("ON ");
  else lcd.print("OFF");

  lcd.setCursor(17, 3);
  if (status.tb4 == 1) lcd.print("ON ");
  else lcd.print("OFF");
}
/************ man hinh cai dat ***********/
void setting_display(void)
{
  if (status.cursor == 0)
  {
    lcd.setCursor(0, 1);
    lcd.print(">");
    lcd.setCursor(0, 2);
    lcd.print(" ");
    lcd.setCursor(0, 3);
    lcd.print(" ");

    if (status.bnt_tb3 != status.old_bnt_tb3)
    {
      setting.volt = setting.volt + 5;
      if (setting.volt > 500) setting.volt = 0;
      status.old_bnt_tb3 = status.bnt_tb3;
    }
    if (status.bnt_tb4 != status.old_bnt_tb4)
    {
      setting.volt = setting.volt - 5;
      if (setting.volt < 1) setting.volt = 500;
      status.old_bnt_tb4 = status.bnt_tb4;
    }
  }
  else if (status.cursor == 1)
  {
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(0, 2);
    lcd.print(">");
    lcd.setCursor(0, 3);
    lcd.print(" ");
    if (status.bnt_tb3 != status.old_bnt_tb3)
    {
      setting.current = setting.current + 2;
      if (setting.current > 100) setting.current = 0;
      status.old_bnt_tb3 = status.bnt_tb3;
    }
    if (status.bnt_tb4 != status.old_bnt_tb4)
    {
      setting.current = setting.current - 2;
      if (setting.current < 1) setting.volt = 100;
      status.old_bnt_tb4 = status.bnt_tb4;
    }
  }
  else if (status.cursor == 2)
  {
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(0, 2);
    lcd.print(" ");
    lcd.setCursor(0, 3);
    lcd.print(">");
    if (status.bnt_tb3 != status.old_bnt_tb3)
    {
      setting.gas = setting.gas + 2;
      if (setting.gas > 100) setting.gas = 0;
      status.old_bnt_tb3 = status.bnt_tb3;
    }
    if (status.bnt_tb4 != status.old_bnt_tb4)
    {
      setting.gas = setting.gas - 2;
      if (setting.gas < 1) setting.gas = 100;
      status.old_bnt_tb4 = status.bnt_tb4;
    }
  }
  if (status.bnt_tb1 != status.old_bnt_tb1)
  {
    status.cursor ++;
    if (status.cursor > 2) status.cursor = 0;
    status.old_bnt_tb1 = status.bnt_tb1;
  }
  lcd.setCursor(7, 0);
  lcd.print("CAI DAT");

  lcd.setCursor(1, 1);
  lcd.print("1. DienAp   : ");

  lcd.setCursor(14, 1);
  char volt[3];
  sprintf(volt, "%03d", setting.volt);
  lcd.print(volt);

  lcd.setCursor(19, 1);
  lcd.print("V");

  lcd.setCursor(1, 2);
  lcd.print("2. DongDien : ");

  lcd.setCursor(14, 2);
  char current[3];
  sprintf(current, "%03d", setting.current);
  lcd.print(current);

  lcd.setCursor(19, 2);
  lcd.print("A");

  lcd.setCursor(1, 3);
  lcd.print("3. Gas      : ");
  lcd.setCursor(14, 3);
  char gas[3];
  sprintf(gas, "%03d", setting.gas);
  lcd.print(gas);

  lcd.setCursor(19, 3);
  lcd.print("%");
}
