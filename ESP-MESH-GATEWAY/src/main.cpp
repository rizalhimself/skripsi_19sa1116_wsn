#include <Arduino.h>
#include <painlessMesh.h>
#include <ArduinoJson.h>

#define MESH_PREFIX "whateverYouLike"
#define MESH_PASSWORD "somethingSneaky"
#define MESH_PORT 5555

Scheduler userScheduler;
painlessMesh mesh;

void sendMessage(); //prototype

int nodeNumber;
float temperature, humidity, sinyal;

Task taskSendMessage (TASK_SECOND * 5, TASK_FOREVER, &sendMessage);

void receivedCallback( uint32_t from, String &msg ) {
  String json;
  DynamicJsonDocument doc(1024);
  json = msg.c_str();
  DeserializationError error = deserializeJson(doc,json);
  Serial.println(json);
  if (error){
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
  }
  nodeNumber = doc["node"];
  temperature = doc["temp"];
  humidity = doc["hum"];
  sinyal = doc["sig"];
  Serial.println(nodeNumber);
  Serial.println(temperature);
  Serial.println(humidity);
  Serial.println(sinyal);
}

void sendMessage(){}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
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
