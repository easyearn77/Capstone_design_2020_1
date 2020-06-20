#include <SoftwareSerial.h>
#include <LedControl.h>
#include <stdlib.h>
#include "LedControl.h" // 라이브러리 사용 선언
#define DEBUG true


const int led = LED_BUILTIN;
int speaker = 9;

// 업로드 알림 LED 설정 (아두이노 우노 On Board LED)
int ledPin = 13;
//pin number
int echoPin=10; 
int trigPin=11;
LedControl lc=LedControl(4,6,5,4);   // Din 핀을 4번, ClK핀을 6번 CS핀을 5번에 연결, 매트릭스는 4개를 사용 선언
int num;


SoftwareSerial esp8266(2, 3); // RX/TX 설정, serial 객체생성
 
void setup() {
  Serial.begin(9600); 
  esp8266.begin(9600);  //시리얼통신속도 9600보드레이트 설정
  
  pinMode(13,OUTPUT);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(led, OUTPUT);
  pinMode(speaker, OUTPUT);
  digitalWrite(led, 0);

  for(num=0; num<4; num++) // 매트릭스 0번부터 3번까지 세팅
  {
   lc.shutdown(num,false); // 0~3번까지 매트릭스 절전모드 해제
   lc.setIntensity(num,8); // 매트릭스의 밝기 선언 0~15의 수
   lc.clearDisplay(num); // 매트릭스 led를 초기화
  }
    
  /*AT Command 이용*/
  sendData("AT+RST\r\n", 2000, DEBUG); //reset module
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG); //dual mode로 설정
  sendData("AT+CWJAP=\"f415a\",\"f415a000\"\r\n", 5000, DEBUG); //사용할 공유기 설정
}
 
void loop() {
  float duration, distance;
  String latitude="37.328707";
  String longitude="127.116459";
  // 초음파를 보낸다. 다 보내면 echo가 HIGH 상태로 대기하게 된다.
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  
  // echoPin 이 HIGH를 유지한 시간을 저장 한다.
  duration = pulseIn(echoPin, HIGH); 
  // HIGH 였을 때 시간(초음파가 보냈다가 다시 들어온 시간)을 가지고 거리를 계산 한다.
  distance = ((float)(340 * duration) / 10000) / 2;  

  //안전바 사이의 거리(50cm)보다 작게 되면 서버에 위치를 전송합니다.
  if(distance<50||distance>2000){
    Serial.print(distance);
    Serial.println("cm // warning");
    Serial.print("longitude: ");
    Serial.print(longitude);
    Serial.print(" latitude: ");
    Serial.println(latitude);
    
    // TCP 연결
    String cmd = "AT+CIPSTART=\"TCP\",\"";
    cmd += "192.168.1.102"; // 서버 IP(local-host사용)
    cmd += "\",8080";           // 포트: 8080
    esp8266.println(cmd);
   
    if(esp8266.find("Error")){
      Serial.println("AT+CIPSTART error");
      return;
    }
  
    // GET 방식으로 보내기 위한 String, Data 설정
    String getStr = "GET /update?=";
    //getStr += apiKey;
    getStr +="&longitude=";
    getStr += String(longitude);//경도
    getStr +="&latitude=";
    getStr += String(latitude);//위도
    getStr += "\r\n\r\n";
 
    // Send Data
    cmd = "AT+CIPSEND=";
    cmd += String(getStr.length());
    esp8266.println(cmd);
 
    if(esp8266.find(">")){
      esp8266.print(getStr);
    }
    else{
      esp8266.println("AT+CIPCLOSE");
      // alert user
      Serial.println("AT+CIPCLOSE");
    }
    
    // 업로드 간격 1초를 맞추기 위한 delay
    delay(1000);  
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


  //speaker 작동
  tone(speaker, 440);
delay(700);
noTone(speaker);
delay(300);//라

tone(speaker, 349);
delay(700);
noTone(speaker);
delay(300);//파

tone(speaker, 392);
delay(700);
noTone(speaker);
delay(300);//솔

tone(speaker, 262);
delay(700);
noTone(speaker);
delay(300);//도

tone(speaker, 262);
delay(700);
noTone(speaker);
delay(300);//도

tone(speaker, 392);
delay(700);
noTone(speaker);
delay(300);//솔

tone(speaker, 440);
delay(700);
noTone(speaker);
delay(300);//라

tone(speaker, 349);
delay(700);
noTone(speaker);
delay(300);//파
  
  }
else
{
  Serial.print("out of sight");
  Serial.print("\n");
  
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
  delay(100);
  }

  /*ESP8266의 정보를 알아내고 설정하기 위한 함수 선언*/
  String sendData(String command, const int timeout, boolean debug){
    String response = "";
    esp8266.print(command); //command를 ESP8266에 보냄
    long int time=millis();
  
    while((time+timeout)>millis()){
      while(esp8266.available()){
        /*esp가 가진 데이터를 시리얼 모니터에 출력하기 위함*/
        char c=esp8266.read(); //다음 문자를 읽어옴
        response+=c;
      }
    }
    if(debug){
      Serial.print(response);
    }
    return response;
  }
