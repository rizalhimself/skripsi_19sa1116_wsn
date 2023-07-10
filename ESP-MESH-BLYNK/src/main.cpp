#define BLYNK_TEMPLATE_ID "TMPLZ11Dwyk4"
#define BLYNK_TEMPLATE_NAME "ESPmeshKumbungJamur"
#define BLYNK_AUTH_TOKEN "kyX2WtzyKbizSfpoH_I77De1rOBneAgT"

#include <Arduino.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <TaskScheduler.h>

#define RXD 3
#define TXD 1
Scheduler userScheduler;

void receiveData(); 
void sendData(); //prototype

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "MF90_452200";
char password[] = "2U346J2J88";
int kel1, kel2, kel3;
float suhu1, suhu2 , suhu3;

Task taskReceieveData(TASK_SECOND * 1, TASK_FOREVER, &receiveData);

void receiveData() {
  String message;
  if(Serial.available()){
    message = Serial.readString();
  }
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, message);
  kel1 = doc["kel1"];
  kel2 = doc["kel2"];
  kel3 = doc["kel3"];
  suhu1 = doc["suhu1"];
  suhu2 = doc["suhu2"];
  suhu3 = doc["suhu3"];
}

void sendData() {
  Blynk.virtualWrite(V0, kel1);
  Blynk.virtualWrite(V1,suhu1);
  Blynk.virtualWrite(V2, kel2);
  Blynk.virtualWrite(V3,suhu2);
  Blynk.virtualWrite(V4, kel3);
  Blynk.virtualWrite(V5, suhu3);
}

void setup() {
  Serial.begin(115200, SERIAL_8N1);
  Blynk.begin(auth,ssid,password);

}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
}

