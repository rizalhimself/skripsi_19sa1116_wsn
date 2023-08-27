#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <painlessMesh.h>
#include <ArduinoJson.h>

const int pinDHT = 2;
const int nodeNumber = 1;

#define DHTTYPE DHT11
#define MESH_PREFIX "whateverYouLike"
#define MESH_PASSWORD "somethingSneaky"
#define MESH_PORT 5555

Scheduler userScheduler;
painlessMesh mesh;
DHT dht(pinDHT, DHTTYPE);
float suhu, kelembapan;

void sendMessage(); // prototype

Task taskSendMessage(TASK_SECOND * 5, TASK_FOREVER, &sendMessage);

void sendMessage()
{
  suhu = dht.readTemperature();
  kelembapan = dht.readHumidity();
  DynamicJsonDocument doc(1024);
  doc["node"] = nodeNumber;
  doc["temp"] = (suhu - 9.8349)/0.728;
  doc["hum"] = (kelembapan-10.635)/0.7081;
  doc["sig"] = WiFi.RSSI();
  String msg;
  serializeJson(doc, msg);
  mesh.sendBroadcast(msg);
}

// Needed for painless library
void receivedCallback(uint32_t from, String &msg)
{
}

void newConnectionCallback(uint32_t nodeId)
{
}

void changedConnectionCallback()
{
}

void nodeTimeAdjustedCallback(int32_t offset)
{
}

void setup()
{
  Serial.begin(115200);
  dht.begin();
  mesh.setDebugMsgTypes(ERROR | STARTUP);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();
}

void loop()
{
  // put your main code here, to run repeatedly:
  mesh.update();
}
