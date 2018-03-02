#include <stdlib.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define ADDR 0x3F
#define Rs 0
#define Rw 1
#define En 2
#define BACKLIGHT 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
LiquidCrystal_I2C lcd(ADDR, En, Rw, Rs, D4, D5, D6, D7);

#define SSID "GANESH_Network"
#define PASS "leeladeen"
#define IP "184.106.153.149"

String GET = "GET /update?key=P6NUL7CSV20DGH2D";
String FIELD1 = "&field1=";
String FIELD2 = "&field2=";

#define BAUD_RATE 115200
#define DELAY_TIME 60000

#define PLIM 2
#define TLIM 40

void setup() {
  Serial.begin(BAUD_RATE);

  Serial.println("AT");

  delay(5000);

  if (Serial.find("OK")) {
    if (connectWiFi() == false) {
      Serial.println("Invalid Network Credentials");
    }
  } else {
    Serial.println("Invalid Network Credentials");
  }

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(7, OUTPUT);
  /* -- LCD Configurations --*/
  lcd.begin(16, 2);
  lcd.setBacklightPin(BACKLIGHT, POSITIVE);
  lcd.setBacklight(HIGH);
}

void loop() {

  /*float pressure = analogRead(A0) * 2;  /*-- 0-5V Output to 0-10bar --*/
  /*float temperature = analogRead(A1) * 20; /*-- 0-5V Output to 0-100degC --*/
  float pressure = 1;
  float temperature = 20;
  if(pressure > PLIM) {
    pinMode(7, 1);
  } else {
    pinMode(7, 0);
  }

  if(temperature > TLIM) {
    pinMode(7, 1);
  } else {
    pinMode(7, 0);
  }
  
  updateValues(String(pressure), String(temperature));
  updateDisplay(pressure, temperature);
  
  delay(DELAY_TIME);
}

boolean connectWiFi() {
    
  Serial.println("AT+CWMODE=1");
  delay(2000);

  String cmd = "AT+CWJAP=\"";
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";

  Serial.println(cmd);
  delay(5000);

  if (Serial.find("OK")) {
    return true;
  } else {
    return false;
  }
}

boolean updateValues(String pressure, String temperature) {
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";

  Serial.println(cmd);
  delay(2000);
  if (Serial.find("Error")) {
    return false;
  }

  cmd = GET;

  cmd += FIELD1;
  cmd += pressure;

  cmd += FIELD2;
  cmd += temperature;

  cmd += "\r\n";

  Serial.print("AT+CIPSEND=");

  Serial.println(cmd.length());

  if (Serial.find(">")) {
    Serial.print(cmd);
  } else {
    Serial.println("AT+CIPCLOSE");
  }

  if (Serial.find("OK")) {
    return true;
  } else {
    return false;
  }
}

void updateDisplay(float pressure, float temperature) {
  lcd.home();
  /*-- State of Compressor --*/
  if (7 == HIGH) {
    lcd.print("Compressor ON");
  } else {
    lcd.print("Compressor OFF");
  }
  lcd.setCursor (0, 1);
  /*-- Display the Parameters --*/
  lcd.print(pressure, 2);
  lcd.print("kg, ");
  /*Round off temperature to 1 digit -> temperature = ((int) (temperature * 10.0 + 0.5) / 10.0);*/
  lcd.print(temperature, 2);
  lcd.print("C");
}
