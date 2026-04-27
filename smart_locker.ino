#include <SoftwareSerial.h>
#include <DFRobot_HuskyLens.h>
#include <HUSKYLENS.h>
#include <HUSKYLENSMindPlus.h>
#include <HuskyLensProtocolCore.h>

#include "HUSKYLENS.h"
#include <Servo.h>
#include <Keypad.h>
#include <Wire.h>

HUSKYLENS huskylens;
Servo servo;

const byte ROWS = 4;
const byte COLS = 4;

byte rowPins[ROWS] = {4, 5, 6, 7};       // 키패드 Row 핀
byte colPins[COLS] = {2, A0, A1, A2};    // 키패드 Col 핀

char keys[ROWS][COLS] = {
  {'D','C','B','A'},
  {'#','9','6','3'},
  {'0','8','5','2'},
  {'*','7','4','1'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int digitPin[3] = {9, 10, 11}; // RGB LED 핀 (R,G,B)
int buzzerPin = 13;

String inputPassword = "";
String correctPassword = "1234";

bool waitingForPassword = false; // 얼굴 인식 실패 후 키패드 대기 상태

void setup() {
  Serial.begin(115200);
  Wire.begin();

  while (!huskylens.begin(Wire)) {
    Serial.println("허스키렌즈 연결 재시도...");
    delay(100);
  }

  for (int i = 0; i < 3; i++) {
    pinMode(digitPin[i], OUTPUT);
  }
  pinMode(buzzerPin, OUTPUT);

  servo.attach(3);
  servo.write(90); // 닫힘 상태
}

void loop() {
  if (!waitingForPassword) {
    // 얼굴 인식 시도
    if (huskylens.request()) {
      if (huskylens.isLearned() && huskylens.available()) {
        bool faceRecognized = false;
        while (huskylens.available()) {
          HUSKYLENSResult result = huskylens.read();
          if (result.command == COMMAND_RETURN_BLOCK && (result.ID >= 1 && result.ID <= 4)) {
            Serial.println("얼굴 인식 성공! 서보 오픈");
            openServo();
            faceRecognized = true;
          }
        }
        if (!faceRecognized) {
          Serial.println("얼굴 인식 실패! 키패드 인증으로 전환");
          waitingForPassword = true;
          inputPassword = "";
        }
      } else { 
        Serial.println("얼굴 인식 실패! 키패드 인증으로 전환");
        waitingForPassword = true;
        inputPassword = "";
      }
    }
  } else {
    // 키패드 입력 처리
    char key = keypad.getKey();
    if (key) {
      Serial.print("Keypad: ");
      Serial.println(key);

      if (key == '*') {
        inputPassword = "";
        Serial.println("입력 초기화됨");
      } else if (key == '#') {
        checkPassword();
      } else {
        inputPassword += key;
        Serial.print("입력 중: ");
        Serial.println(inputPassword);
      }
    }
  }
}

// 서보 열기 후 3초 유지하고 닫기
void openServo() {
  servo.write(0); // 열기
  Serial.println("서보 열림");

  tone(buzzerPin, 262, 500); delay(500);
  tone(buzzerPin, 330, 500); delay(500);
  tone(buzzerPin, 392, 500); delay(500);
  noTone(buzzerPin);

  setRGB(0, 255, 0);  // 초록 LED 켜기
  delay(3000);        // 3초간 열림 유지

  servo.write(90);    // 닫기
  Serial.println("서보 닫힘");

  setRGB(0, 0, 0);    // LED 끄기
}

// 비밀번호 체크 함수
void checkPassword() {
  if (inputPassword == correctPassword) {
    Serial.println("비밀번호 일치: 서보 오픈");
    openServo();
  } else {
    Serial.println("비밀번호 틀림!");
    setRGB(255, 0, 0);  // 빨강 LED 켜기
    tone(buzzerPin, 300, 500);
    delay(1000);
    noTone(buzzerPin);
    setRGB(0, 0, 0);
  }

  inputPassword = "";
  waitingForPassword = false; // 다시 얼굴 인식 대기 상태로 전환
}

// RGB LED 색상 설정 함수
void setRGB(int r, int g, int b) {
  analogWrite(digitPin[0], r);
  analogWrite(digitPin[1], g);
  analogWrite(digitPin[2], b);
}

// 얼굴 인식 성공 시 서보 모터 회전
// 얼굴 인식 실패 시 -> 키패드 비밀번호 입력