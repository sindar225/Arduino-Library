#define THINGER_SERVER "138.201.45.114"
#define _DEBUG_
#define _DISABLE_TLS_

#include <ESP8266WiFi.h>
#include <ThingerSmartConfig.h>

#define MQ2pin A0

#define USERNAME "sindar225"
#define DEVICE_ID "Kitchen_ESP8266"
#define DEVICE_CREDENTIAL "m5m@Rhp9H2kw"

float sensorValue;  // переменная для хранения значения датчика
ThingerSmartConfig thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL, false);

void setup() {
  Serial.begin(9600);
  //Serial.println("Gas sensor warming up!");
  
  thing["air_bucket"] >> [](pson &out){ 
    out["air_quality"] = getSensorValue();
  };

  thing["air_quality"] >> outputValue(getSensorValue());
}

void loop() {
  pson data;
  data["air_quality"] = getSensorValue();
  data["location"] = DEVICE_ID;

  thing.handle();
  if (int(getSensorValue()) >= 300) {
    Serial.print("Sending message to TG");
    thing.call_endpoint("telegramAlert", data);
  }  
  thing.write_bucket("Air_quality", "air_bucket");
  ESP.deepSleep(20e6, WAKE_RF_DEFAULT);
}

float getSensorValue() {
  sensorValue = analogRead(MQ2pin); // прочитать аналоговый вход 0
  return sensorValue;
}
