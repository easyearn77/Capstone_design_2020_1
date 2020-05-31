//스피커 모양 앰프를 lm386에 꽂고 준비,
//스피커 모듈 vcc-> wemos 기기의 5v, gnd는 gnd, In은 tx1/d9핀에 연결


int speaker = 2; //wemos d1의 tx1/d9 핀에 해당

void setup() {

pinMode(speaker, OUTPUT);

}

//스쿨존에 적용되는 프로젝트인 만큼 경고음은 학교 종소리로 구현
void loop() {

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

//결과: 라파솔도 도솔라파가 계속 재생됨(생각보다 소리 매우 큼)
