# 🛡️ ESP32-C6 Telegram-Controlled Door Lock

This project allows remote control of a 270° servo-powered door lock using a Telegram bot and an ESP32-C6 (Seeed Studio XIAO). Multiple authorized users can operate the lock, and the door automatically closes after 10 seconds.

---

## 📦 Hardware

| Component | Description | Link |
|----------|-------------|------|
| Microcontroller | Seeed Studio XIAO ESP32-C6 | [XIAO ESP32-C6 Getting Started](https://wiki.seeedstudio.com/xiao_esp32c6_getting_started/) |
| Current Compatible Servo | DS3235-like 270° Servo | [Amazon Link](https://a.co/d/hh7hMv6) |
| New Servo (STL not yet updated) | MG996R 180° Servo | [Amazon Link](https://a.co/d/5FDcMUa) |
| STL Design Files | 3D printable lock hardware | [STL 1](https://a.co/d/8S7qjCs) / [STL 2](https://a.co/d/4TOVGFS) / [STL 3](https://a.co/d/gJ13i0S) |

---

## 🧠 Features

- 🛡️ Secure access with multiple authorized Telegram users
- 🔓 Opens servo-controlled lock remotely via `/open` command
- ⏱️ Automatically closes after 10 seconds
- 🔁 Status, help, and notifications for all users
- 🔌 Relay control for power management
- 🔇 Servo detach logic for power saving

---

## ⚙️ Getting Started

### 1. Telegram Bot via BotFather

1. Open Telegram, search for [BotFather](https://core.telegram.org/bots#botfather)
2. Type `/newbot` and follow the instructions
3. Save your **Bot Token**

📖 [Full BotFather Documentation](https://core.telegram.org/bots#botfather)

---

### 2. Arduino Setup for ESP32-C6

Follow the Seeed Studio guide:
👉 [XIAO ESP32-C6 Getting Started](https://wiki.seeedstudio.com/xiao_esp32c6_getting_started/)

Ensure:
- ESP32 board package is installed
- You select the correct board/port
- Libraries installed (see below)

---

### 3. Arduino Libraries Required

Install the following via Library Manager:
- `WiFi.h`
- `WiFiClientSecure.h`
- `UniversalTelegramBot`
- `ArduinoJson`
- `ESP32Servo`

---

## 📐 Pin Connections

| Component     | ESP32-C6 Pin | Notes               |
|---------------|--------------|---------------------|
| Relay         | D18 / GPIO18 | Powers the servo    |
| Servo Signal  | D17 / GPIO17 | DS3235 270° Servo   |
| LED Indicator | D15 / GPIO15 | Optional status LED |

> ⚠️ Use a separate 5V power supply for your servo to prevent brownouts.

---

## 🧾 Code

```cpp
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

// WiFi credentials
const char* ssid = "wifi_name";
const char* password = "wifi_password";

// Telegram Bot
#define BOT_TOKEN "token_from_bot_father"
const String AUTHORIZED_USERS[] = {"users_id"}; // Add real user IDs
const int NUM_AUTHORIZED_USERS = 1;

// Pins
const int RELAY_PIN = 18;
const int SERVO_PIN = 17;
const int LED_PIN = 15;

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

Servo ds3235;
bool servoAttached = false;
bool relayState = false;
bool servoIsOpen = false;
unsigned long servoOpenTime = 0;
const unsigned long servoOpenDuration = 10000;

unsigned long lastTimeBotRan;
const int botRequestDelay = 1000;

const int closedPosition = 0;
const int openPosition = 270;
