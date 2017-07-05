#include <stdlib.h>
#include "DHT.h"

#define SSID "GANESH_Network"
#define PASS "leeladeen"
#define IP "184.106.153.149"
#define DHTPIN 3
#define DHTTYPE DHT11
#define carbonMonoxidePin 1
#define methanePin 2
#define Baud_Rate 115200
#define DELAY_TIME 60000

String GET = "GET /update?key=RXRBVU0E52VNNMS8&field1=";

String FIELD2 = "&field2=";
String FIELD3 = "&field3=";
String FIELD4 = "&field4=";
//String FIELD5 = "&field5=";
//String FIELD6 = "&field6=";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  
  Serial.begin(Baud_Rate);

  pinMode(carbonMonoxidePin, INPUT);
  pinMode(methanePin, INPUT);

  Serial.println("AT");

  delay(5000);

  if (Serial.find("OK")) {
    if (connectWiFi() == false) {
      Serial.println("Invalid Network Credentials");
    }
  } else {
    Serial.println("Invalid Network Credentials");
  }
  
  dht.begin();
}

void loop() {

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  float methaneAmt = methaneValue();

  float carbonMonoxideAmt = carbonMonoxideValue();

  updateValues(String(temperature), String(humidity), String(methaneAmt), String(carbonMonoxideAmt));

  delay(DELAY_TIME);
}

void updateValues(String temperature, String humidity, String methaneAmt, String carbonMonoxideAmt) {

  String cmd = "AT+CIPSTART=\"TCP\",\"";

  cmd += IP;
  cmd += "\",80";

  Serial.println(cmd);
  delay(2000);
  if (Serial.find("Error")) {
    return false;
  }

  cmd = GET;
  cmd += temperature;
  
  cmd += FIELD2;
  cmd += humidity;

  cmd += FIELD3;
  cmd += methaneAmt;
  
  cmd += FIELD4;
  cmd += carbonMonoxideAmt;

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

float carbonMonoxideValue() {
  float sensorValue = analogRead(carbonMonoxidePin);
  return sensorValue / 2048 * 25.0;
}

float methaneValue() {
  float sensorValue = analogRead(methanePin);
  return sensorValue / 1024 * 1.2;
}
