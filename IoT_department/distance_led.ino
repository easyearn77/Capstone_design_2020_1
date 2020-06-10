#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "LedControl.h" // 라이브러리 사용 선언


#ifndef STASSID
#define STASSID "put ur wifi name here"
#define STAPSK  "put ur wifi's pw here"
#include "LedControl.h" // 라이브러리 사용 선
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int led = LED_BUILTIN;

//pin number
int echoPin=16; //wemos->D2
int trigPin=15; //wemos->D10 
LedControl lc=LedControl(12,14,13,4);   // Din 핀을 12번, ClK핀을 14번 CS핀을 13번에 연결, 매트릭스는 4개를 사용 선언
int num;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  
  for(num=0; num<4; num++) // 매트릭스 0번부터 3번까지 세팅
  {
   lc.shutdown(num,false); // 0~3번까지 매트릭스 절전모드 해제
   lc.setIntensity(num,8); // 매트릭스의 밝기 선언 0~15의 수
   lc.clearDisplay(num); // 매트릭스 led를 초기화
  }
  


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
    Serial.print("\n");

   byte m[8]={
             B11111111,
             B11000000,
             B11000000,
             B11000000,
             B11111111,
             B00000011,
             B00000011,
             B11111111
             };
  //'T'
 byte t[8]={
            B11111111,
            B11111111,
            B00111000,
            B00111000,
            B00111000,
            B00111000,
            B00111000,
            B00111000
            };


 // 'O'
 byte r[8]={
            B01111110,
            B01000010,
            B01000010,
            B01000010,
            B01000010,
            B01000010,
            B01000010,
            B01111110
           };
  //'P'
 byte x[8]={
            B11111111,
            B11000011,
            B11000011,
            B11111111,
            B11000000,
            B11000000,
            B11000000,
            B11000000
            };


 // lc.setRow 함수는 행(Row) 기준으로 도트매트릭스를 제어 lc.setRow(matrix_number,Row,value)
 for(int j=0; j<8; j++)
 {
   lc.setRow(3,j,m[j]); // stop 한 글자씩 led 판에 출력 
   lc.setRow(2,j,t[j]); 
   lc.setRow(1,j,r[j]); 
   lc.setRow(0,j,x[j]);
 }
 delay(300);
 


}
else
{
  Serial.print("out of sight");
  Serial.printf("\n");
  
   byte m[8]={
             B00000000,
             B00000000,
             B00000000,
             B00000000,
             B00000000,
             B00000000,
             B00000000,
             B00000000
             };
  //'T'
 byte t[8]={
             B00000000,
             B00000000,
             B00000000,
             B00000000,
             B00000000,
             B00000000,
             B00000000,
             B00000000
             };


 // 'O'
 byte r[8]={
             B00000000,
             B00000000,
             B00000000,
             B00000000,
             B00000000,
             B00000000,
             B00000000,
             B00000000
             };
  //'P'
 byte x[8]={
             B00000000,
             B00000000,
             B00000000,
             B00000000,
             B00000000,
             B00000000,
             B00000000,
             B00000000
             };

 // lc.setRow 함수는 행(Row) 기준으로 도트매트릭스를 제어 lc.setRow(matrix_number,Row,value)
 for(int j=0; j<8; j++)
 {
   lc.setRow(3,j,m[j]); // stop 한 글자씩 led 판에 출력 
   lc.setRow(2,j,t[j]); 
   lc.setRow(1,j,r[j]); 
   lc.setRow(0,j,x[j]);
 }
 delay(300);
 
}
  delay(500);
}
