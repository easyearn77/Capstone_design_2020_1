#include <SoftwareSerial.h>
#include <stdlib.h>
#define DEBUG true

 
// 업로드 알림 LED 설정 (아두이노 우노 On Board LED)
int ledPin = 13;
//pin number
int echoPin=10; 
int trigPin=11;
 

SoftwareSerial esp8266(2, 3); // RX/TX 설정, serial 객체생성
 
void setup() {
  Serial.begin(9600); 
  esp8266.begin(9600);  //시리얼통신속도 9600보드레이트 설정
  
  pinMode(13,OUTPUT);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
    
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
  }
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
