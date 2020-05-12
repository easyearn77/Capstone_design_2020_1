#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "only jieon"
#define STAPSK  "wldjs8702"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int led = LED_BUILTIN;

//pin number
int echoPin=12; //wemos->D6
int trigPin=13; //wemos->D7

void setup() {
  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  float duration, distance;
  server.handleClient();
  MDNS.update();
  
  // 초음파를 보낸다. 다 보내면 echo가 HIGH 상태로 대기하게 된다.
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  
  // echoPin 이 HIGH를 유지한 시간을 저장 한다.
  duration = pulseIn(echoPin, HIGH); 
  // HIGH 였을 때 시간(초음파가 보냈다가 다시 들어온 시간)을 가지고 거리를 계산 한다.
  distance = ((float)(340 * duration) / 10000) / 2;  

  if(distance<50||distance>2000){
    Serial.print(distance);
    Serial.println("cm // warning");
  }
  

  // 수정한 값을 출력
  delay(50);
}
