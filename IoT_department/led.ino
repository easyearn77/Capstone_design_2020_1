/*
8x32 dot matrix
Ino 열어 툴-> 라이브러리 관리 메뉴 들어가서 ledcontrol치고
첫번째로 나오는것 설치
*/


#include "LedControl.h" // 라이브러리 사용 선언


// wemos d1 보드 기준으로 Din 핀을 12번(d12/miso/d6), ClK핀을 14번(d13/sck/d5) CS핀을 13번(d11/mosi/d7)에 연결, 매트릭스는 4개를 사용 선언
LedControl lc=LedControl(12,14,13,4); 
int num;


void setup()
{
 for(num=0; num<4; num++) // 매트릭스 0번부터 3번까지 세팅
  {
   lc.shutdown(num,false); // 0~3번까지 매트릭스 절전모드 해제
   lc.setIntensity(num,8); // 매트릭스의 밝기 선언 0~15의 수
   lc.clearDisplay(num); // 매트릭스 led를 초기화
  }
}


// matrix 함수 선언, 운전자에게 경고 메세지 주기 위해 'STOP' 이라는 메세지를 나타내도록 구현함(메세지 내용은 추후 변경 가능성O) 
void matrix()
{
  // 'S'
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
   lc.setRow(3,j,m[j]); // 0번째 매트릭스에서 'S'출력
   lc.setRow(2,j,t[j]); // 1번째 매트릭스에서 'T'출력
   lc.setRow(1,j,r[j]); // 2번째 매트릭스에서 'O'출력
   lc.setRow(0,j,x[j]); // 3번째 매트릭스에서 'P'출력
 }
 delay(1000);
}


void loop() {
  matrix();
  for(num=0; num<4; num++) // 4개의 매트릭스led 초기화
 {
  lc.clearDisplay(num);
 }
 delay(1000);


}

//결과: led에 STOP이라는 문구가 깜박거림
