#include <DS1302.h>
#include <SPI.h>
#include <TFT.h>

#define cs 10
#define dc 9
#define rst 8
#define SOUND_SENSOR_1  A3 // 사운드 센서1 핀
#define SOUND_SENSOR_2  A1 // 사운드 센서2 핀
#define vib 3
#define RED 2             // led R핀 
#define GREEN 4           // led G핀

int a; //센서1 상태 저장 변수
int b; //센서2 상태 저장 변수

TFT TFTscreen = TFT(cs, dc, rst);

DS1302 RTC(7, 6, 5);

void setup() {
	Serial.begin(9600);
	RTC.halt(false);
	RTC.writeProtect(false);
	RTC.setDOW(THURSDAY);		// RTC 모듈 요일 설정
	RTC.setTime(16, 26, 00);		// RTC 모듈 시간 설정
	RTC.setDate(31, 5, 2022);		// RTC 모듈 날짜 설정

	TFTscreen.begin();			// TFT LCD 시작
	TFTscreen.setRotation(0);		// TFT LCD 화면 회전
	TFTscreen.background(0, 0, 0);		// TFT LCD 뒷배경 색 설정 (검정색)
	TFTscreen.stroke(255, 255, 255);	// TFT LCD 글자 색 설정 (흰색)

	pinMode(A3, INPUT);       // 사운드 센서1 입력으로 설정
	pinMode(A1, INPUT);       // 사운드 센서2 입력으로 설정
	pinMode(vib, OUTPUT);     // 진동 센서 출력으로 설정
	pinMode(RED, OUTPUT);     // led R핀 출력으로 설정
	pinMode(GREEN, OUTPUT);  	// led G핀 출력으로 설정
}

void loop() {
	a = analogRead(SOUND_SENSOR_1);  //센서1 상태변수 지정
	b = analogRead(SOUND_SENSOR_2);  //센서2 상태변수 지정
	Serial.print("a=");              //시리얼 모니터 출력
	Serial.print(a);
	Serial.print(" b=");
	Serial.println(b);

	analogWrite(GREEN, 500); // LED GREEN 출력

	TFTscreen.stroke(255, 255, 255);
	Time now = RTC.getTime();		// RTC 모듈의 현재 시각 변수로 설정
	TFTscreen.setTextSize(1); 		// TFT LCD 글자 크기 설정
	TFTscreen.setCursor(30, 50);		// TFT LCD 출력 커서 설정
	TFTscreen.print(now.year);		// LCD에 날짜(년) 출력                                                                       
	TFTscreen.print("/");
	TFTscreen.print(now.mon);		// LCD에 날짜(월) 출력                                                                             
	TFTscreen.print("/");
	TFTscreen.print(now.date);		// LCD에 날짜(일) 출력                                                                          
	TFTscreen.print("  ");
	TFTscreen.setTextSize(2);
	TFTscreen.setCursor(15, 80);
	if (now.hour < 10) {
		TFTscreen.print("0");
	}
	TFTscreen.print(now.hour);		// LCD에 시간(시) 출력                                                                             
	TFTscreen.print(":");
	if (now.min < 10) {
		TFTscreen.print("0");
	}
	TFTscreen.print(now.min);		// LCD에 시간(분) 출력                                                                          
	TFTscreen.print(":");
	if (now.sec < 10) {
		TFTscreen.print("0");
	}//| 
	TFTscreen.println(now.sec);		// LCD에 시간(초) 출력

	delay(500);

	TFTscreen.stroke(0, 0, 0);
	TFTscreen.setTextSize(1);
	TFTscreen.setCursor(35, 50);
	TFTscreen.print(now.year);
	TFTscreen.print("/");
	TFTscreen.print(now.mon);
	TFTscreen.print("/");
	TFTscreen.print(now.date);
	TFTscreen.print("  ");
	TFTscreen.setTextSize(2);
	TFTscreen.setCursor(15, 80);
	if (now.hour < 10) {
		TFTscreen.print("0");
	}
	TFTscreen.print(now.hour);
	TFTscreen.print(":");
	if (now.min < 10) {
		TFTscreen.print("0");
	}
	TFTscreen.print(now.min);
	TFTscreen.print(":");
	if (now.sec < 10) {
		TFTscreen.print("0");
	}// 
	TFTscreen.println(now.sec);

	if (a > 600 || b > 600) { //소음수준 감지

		TFTscreen.stroke(0, 0, 0);
		TFTscreen.setTextSize(1);
		TFTscreen.setCursor(30, 50);
		TFTscreen.print(now.year);
		TFTscreen.print("/");
		TFTscreen.print(now.mon);
		TFTscreen.print("/");
		TFTscreen.print(now.date);
		TFTscreen.print("  ");
		int i;
		analogWrite(GREEN, 0);		// LED GREEN 출력
		for (i = 0; i < 10; i++) {
			analogWrite(vib, 1000); 		// 진동 출력
			analogWrite(RED, 200); 		// LED RED 출력
			TFTscreen.setTextSize(2);
			TFTscreen.stroke(0, 0, 255);		// LCD 글자 색 설정(빨강)
			TFTscreen.text("Warning ! ", 15, 80);	// 소음 수준이 일정 치 초과할 시 문구 출력
			delay(300);
			analogWrite(vib, 0);
			analogWrite(RED, 0);
			TFTscreen.stroke(0, 0, 0);
			TFTscreen.text("Warning ! ", 15, 80);

			delay(100);
			a = analogRead(SOUND_SENSOR_1);  //센서1 상태변수 지정
			b = analogRead(SOUND_SENSOR_2);  //센서2 상태변수 지정


			Serial.print("a=");              //시리얼 모니터 출력
			Serial.print(a);
			Serial.print(" b=");
			Serial.println(b);
		}
	}
}