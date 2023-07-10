#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <painlessMesh.h>
#include <ArduinoJson.h>

const int pinDHT = 2;
int nodeNumber = 3;

#define DHTTYPE DHT11
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

Scheduler userScheduler;
painlessMesh mesh;
DHT dht (pinDHT, DHTTYPE);

void sendMessage(); //prototype

Task taskSendMessage (TASK_SECOND * 5, TASK_FOREVER, &sendMessage);

void sendMessage() 
{
  DynamicJsonDocument doc(1024);
  doc["node"] = nodeNumber;
  doc["temp"] = dht.readTemperature();
  doc["hum"] = dht.readHumidity();
  doc["sig"] = WiFi.RSSI();
  String msg;
  serializeJson(doc, msg);
  mesh.sendBroadcast(msg);
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {

}

void newConnectionCallback(uint32_t nodeId) {
}

void changedConnectionCallback() {
}

void nodeTimeAdjustedCallback(int32_t offset) {
}

void setup() {
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

void loop() {
  // put your main code here, to run repeatedly:
  mesh.update();
}
