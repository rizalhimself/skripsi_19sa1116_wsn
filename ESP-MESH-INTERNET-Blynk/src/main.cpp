#define BLYNK_TEMPLATE_ID "TMPLZ11Dwyk4"
#define BLYNK_TEMPLATE_NAME "ESPmeshKumbungJamur"
#define BLYNK_AUTH_TOKEN "kyX2WtzyKbizSfpoH_I77De1rOBneAgT"

#define BLYNK_PRINT Serial
#define APP_DEBUG

#include <Arduino.h>
#include <ArduinoJson.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>

BlynkTimer timer;

void receiveData(); 
void sendData(); //prototype

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "cimmiy";
char password[] = "arsLnstwn18";
int kel1, kel2, kel3;
float suhu1, suhu2 , suhu3;

// put function declarations here:
void receieveData()
{
  String message;
  if(Serial.available())
  {
    message = Serial.readString();
  }
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, message);
  kel1 = doc["kel1"];
  kel2 = doc["kel2"];
  kel3 = doc["kel3"];
  suhu1 = doc["suhu1"];
  suhu2 = doc["suhu2"];
  suhu3 = doc["suhu3"];
}

void sendData() {
  Blynk.virtualWrite(V0, kel1);
  Blynk.virtualWrite(V1, suhu1);
  Blynk.virtualWrite(V2, kel2);
  Blynk.virtualWrite(V3, suhu2);
  Blynk.virtualWrite(V4, kel3);
  Blynk.virtualWrite(V5, suhu3);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200, SERIAL_8N1);
  Blynk.begin(auth,ssid,password);
  timer.setInterval(1000L, receieveData);
  timer.setInterval(3000L, sendData);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();
}