#define THINGER_SERVER ""

#include <ESP8266WiFi.h>
#include <ThingerESP8266.h>

#define MQ2pin A0

#define USERNAME ""
#define DEVICE_ID ""
#define DEVICE_CREDENTIAL ""

#define SSID ""
#define SSID_PASSWORD ""

float sensorValue;  // переменная для хранения значения датчика
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {
  Serial.begin(9600);
  Serial.println("Gas sensor warming up!");
  //delay(20000);       // дать MQ-2 время для прогрева
  //delay(2000);

  thing.add_wifi(SSID, SSID_PASSWORD);

  // resource output example (i.e. reading a sensor value)
  thing["air_quality"] >> outputValue(getSensorValue());

}

void loop() {
  pson data;
  data["air_quality"] = getSensorValue();
  if (int(getSensorValue()) >= 300) {
    thing.call_endpoint("telegramAlert", getSensorValue());
  }

  thing.handle();
  ESP.deepSleep(20e6);
}

float getSensorValue() {
  sensorValue = analogRead(MQ2pin); // прочитать аналоговый вход 0
  return sensorValue;
}
