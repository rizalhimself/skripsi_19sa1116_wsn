#include <Arduino.h>
#include <painlessMesh.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define MESH_PREFIX "whateverYouLike"
#define MESH_PASSWORD "somethingSneaky"
#define MESH_PORT 5555
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 screen(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Scheduler userScheduler;
painlessMesh mesh;

void sendMessage(); 
void sendDataToSerial1();//prototype

int nodeNumber, node1, node2, node3, sinyal, sinyal1, sinyal2, sinyal3,
humidity, kel1, kel2, kel3;
float temperature, suhu1, suhu2 , suhu3;

Task taskSendMessage (TASK_SECOND * 5, TASK_FOREVER, &sendMessage);
Task taskSendData (TASK_SECOND * 3, TASK_FOREVER,&sendDataToSerial1 );

void sendDataToSerial1()
{
  DynamicJsonDocument doc(1024);
  doc["kel1"] = kel1;
  doc["kel2"] = kel2;
  doc["kel3"] = kel3;
  doc["suhu1"] = suhu1;
  doc["suhu2"] = suhu2;
  doc["suhu3"] = suhu3;
  serializeJson(doc,Serial1);
}

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
  if (nodeNumber == 1) {
    node1 = nodeNumber;
    suhu1 = temperature;
    kel1 = humidity;
    sinyal1 = sinyal;
    screen.setCursor(0,0);
    screen.print("Node1");
    screen.setCursor(0,10);
    screen.setTextColor(WHITE,BLACK);
    screen.print("Sg:");
    screen.print(sinyal1);
    screen.setCursor(0,20);
    screen.setTextColor(WHITE,BLACK);
    screen.print("Kl:");
    screen.print(kel1);
    screen.setCursor(0,30);
    screen.setTextColor(WHITE,BLACK);
    screen.print("S:");
    screen.print(suhu1,1);
    screen.display();
  } else if (nodeNumber == 2) {
    node2 = nodeNumber;
    suhu2 = temperature;
    kel2 = humidity;
    sinyal2 = sinyal;
    screen.setCursor(45,0);
    screen.print("Node2");
    screen.setCursor(45,10);
    screen.setTextColor(WHITE,BLACK);
    screen.print("Sg:");
    screen.print(sinyal2);
    screen.setCursor(45,20);
    screen.setTextColor(WHITE,BLACK);
    screen.print("Kl:");
    screen.print(kel2);
    screen.setCursor(45,30);
    screen.setTextColor(WHITE,BLACK);
    screen.print("S:");
    screen.print(suhu2,1);
    screen.display();
  } else if (nodeNumber == 3) {
    node3 = nodeNumber;
    suhu3 = temperature;
    kel3 = humidity;
    sinyal3 = sinyal;
    screen.setCursor(90,0);
    screen.print("Node3");
    screen.setCursor(90,10);
    screen.setTextColor(WHITE,BLACK);
    screen.print("Sg:");
    screen.print(sinyal3);
    screen.setCursor(90,20);
    screen.setTextColor(WHITE,BLACK);
    screen.print("Kl:");
    screen.print(kel3);
    screen.setCursor(90,30);
    screen.setTextColor(WHITE,BLACK);
    screen.print("S:");
    screen.print(suhu3,1);
    screen.display();
  }
}

void sendMessage(){}

void newConnDisplay()
{
  screen.setCursor(0,50);
  screen.setTextColor(BLACK,BLACK);
  screen.display();
  screen.setCursor(0,50);
  screen.setTextColor(WHITE,BLACK);
  screen.printf("I:New conn");
  screen.display();
}

void changeConnDisplay()
{
  screen.setCursor(0,50);
  screen.setTextColor(BLACK,BLACK);
  screen.display();
  screen.setCursor(0,50);
  screen.setTextColor(WHITE,BLACK);
  screen.printf("I:Chgd conn");
  screen.display();
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
  newConnDisplay();
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
  changeConnDisplay();
  
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1);
  // initialize the OLED object
  if (!screen.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Clear the buffer.
  screen.clearDisplay();

  // Display Text
  screen.setTextSize(1);
  screen.setTextColor(WHITE);
  screen.setCursor(25, 28);
  screen.println("Loading...");
  screen.display();
  screen.clearDisplay();
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask(taskSendMessage);
  userScheduler.addTask(taskSendData);
  taskSendMessage.enable();
  taskSendData.enable();
}

void loop() {
  // put your main code here, to run repeatedly:
  mesh.update();
}
