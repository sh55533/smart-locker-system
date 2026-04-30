# 🔐 얼굴 인식 기반 스마트 무인 택배 보관함

![Arduino](https://img.shields.io/badge/Arduino-00979D?style=flat&logo=arduino&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=cplusplus&logoColor=white)
![IoT](https://img.shields.io/badge/IoT-Embedded-green)
![HuskyLens](https://img.shields.io/badge/HuskyLens-AI%20Vision-blue)

> 얼굴 인식과 다중 인증을 결합한 IoT 기반 스마트 택배 보관함 시스템

🎥 **[시연 영상 보기](https://www.youtube.com/watch?v=ODlTaD_d4Dw)**

---

## 📌 프로젝트 개요

전자상거래 증가로 인한 택배 분실·도난 문제를 해결하기 위해, 얼굴 인식 + 키패드 이중 인증 방식의 IoT 택배 보관함을 팀 프로젝트로 개발했습니다.

| 항목 | 내용 |
|------|------|
| 개발 기간 | 2025.03 ~ 2025.10 |
| 팀 구성 | 팀 프로젝트 |
| 담당 역할 | **Arduino 펌웨어 개발, HuskyLens 얼굴인식 모듈 연동** |
| 개발 환경 | Arduino IDE, C++ |

---

## 🛠️ 기술 스택

| 분류 | 사용 기술 |
|------|----------|
| **MCU** | Arduino |
| **AI 비전** | HuskyLens (얼굴 인식) |
| **액추에이터** | 서보모터 (PWM 제어) |
| **인증** | 키패드 (보조 인증) |
| **통신** | Bluetooth (HC-05) |
| **언어** | C++ (Arduino) |

---

## 🔄 시스템 흐름

```
[HuskyLens 얼굴 인식]
        │
        ▼
  인증 성공? ──── NO ──▶ [키패드 보조 인증]
        │                       │
       YES                     YES
        │                       │
        └──────────┬────────────┘
                   ▼
          [Arduino 제어 신호 전송]
                   │
                   ▼
          [서보모터 작동 → 보관함 개폐]
```

---

## ⚙️ 핵심 구현 (담당 파트)

### 1. HuskyLens 얼굴 인식 연동
- I2C 통신으로 HuskyLens와 Arduino 연결
- 등록된 얼굴 ID 매칭 로직 구현
- 인식 성공/실패에 따른 분기 처리

### 2. PWM 기반 서보모터 제어
- 개폐 각도 조절을 위한 PWM 신호 생성
- 보관함 열림/잠금 상태 관리

### 3. 인터럽트 기반 Bluetooth 통신 처리
- 메인 제어 루프와의 충돌 방지를 위해 인터럽트 구조 재설계
- **상태 기반(State Machine) 제어 방식으로 전환**하여 안정성 확보

---

## 🔧 트러블슈팅

### ① 전원 불안정 문제
- **증상**: 장시간 동작 시 보관함이 멈추는 현상 발생
- **원인 분석**: 서보모터 + Arduino + HuskyLens 동시 구동 시 전류 부족
- **해결**: 전원 라인 분리 후 서보모터에 외부 DC 전원 별도 공급

### ② Bluetooth 인터럽트 충돌
- **증상**: Bluetooth 수신 중 제어 동작 오류 발생
- **원인 분석**: 수신 인터럽트와 메인 루프 간 타이밍 충돌
- **해결**: 인터럽트 핸들러 경량화 + 플래그 방식의 상태 기반 제어로 재설계

---

## 📂 파일 구조

```
smart-locker-system/
├── smart_locker.ino     # 메인 펌웨어 (Arduino)
└── README.md
```

---

## ✅ 구현 결과

-  HuskyLens 기반 얼굴 인식 인증 정상 동작
-  키패드 보조 인증 기능 구현
-  서보모터 개폐 제어 안정적 동작
-  Bluetooth Terminal 앱을 통한 원격 제어 기능 구현 완료 (전용 앱 UI 개발은 추후 개선 과제)
-  전원 분리 후 장시간 안정 동작 검증

---

## 💡 배운 점

- 하드웨어-소프트웨어 통합 설계에서 **전원 설계의 중요성** 체감
- **인터럽트 충돌** 문제를 직접 디버깅하며 상태 기반 제어 패턴 학습
- 여러 모듈(HuskyLens, 서보, BT)을 하나의 시스템으로 통합하는 **시스템 통합 역량** 향상
