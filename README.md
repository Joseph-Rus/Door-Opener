# ESP32-C6 Telegram Door Control System

A secure, remote door control system using ESP32-C6 and Telegram Bot API. Control your door lock from anywhere in the world using your smartphone!

![ESP32-C6 Door Control](https://img.shields.io/badge/ESP32--C6-Telegram%20Bot-blue) ![License](https://img.shields.io/badge/license-MIT-green) ![Arduino IDE](https://img.shields.io/badge/Arduino%20IDE-Compatible-orange)

## 📋 Table of Contents
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Hardware Setup](#hardware-setup)
- [Software Setup](#software-setup)
- [Telegram Bot Setup](#telegram-bot-setup)
- [Arduino IDE Configuration](#arduino-ide-configuration)
- [Installation](#installation)
- [Usage](#usage)
- [Commands](#commands)
- [Wiring Diagram](#wiring-diagram)
- [3D Printed Parts](#3d-printed-parts)
- [Troubleshooting](#troubleshooting)
- [Security Features](#security-features)
- [Contributing](#contributing)
- [License](#license)

## ✨ Features

- **Remote Control**: Control your door from anywhere with internet connection
- **Secure Access**: Multi-user authorization with unique Telegram user IDs
- **Auto-lock**: Automatic door closure after 10 seconds
- **Real-time Notifications**: All authorized users get notified when door is opened
- **Status Monitoring**: Check door status and WiFi information
- **Power Management**: Optimized servo control to prevent overheating
- **Battery Support**: Built-in battery charging and monitoring
- **Fail-safe Design**: WiFi reconnection and error handling

## 🛠 Hardware Requirements

### Core Components
- **[Seeed Studio XIAO ESP32-C6](https://www.seeedstudio.com/Seeed-Studio-XIAO-ESP32C6-p-5884.html)** - Main microcontroller
- **[SONOFF 5V Relay Module](https://a.co/d/8S7qjCs)** - Controls power to servo
- **[Rechargeable Battery Pack](https://a.co/d/4TOVGFS)** - Backup power
- **[2.4G WiFi Antenna](https://a.co/d/gJ13i0S)** - Enhanced WiFi range

### Servo Motors (Choose One)
- **Current Compatible**: [DS3235MG Servo](https://a.co/d/hh7hMv6) *(STL files available)*
- **Updated Model**: [High Torque Servo](https://a.co/d/5FDcMUa) *(STL files need updating)*

### Additional Components
- Jumper wires
- Breadboard or PCB
- 3.7V Lithium battery (optional)
- External antenna (optional for better range)

## 💻 Software Requirements

- **Arduino IDE** (version 1.8.13 or later)
- **ESP32 Board Package** (version 3.0.0 or later)
- **Required Libraries**:
  - UniversalTelegramBot
  - ArduinoJson (version 6.15.2 or later)
  - ESP32Servo
  - WiFiClientSecure

## 🔧 Hardware Setup

### ESP32-C6 Setup

The **Seeed Studio XIAO ESP32-C6** is a powerful, compact development board perfect for IoT projects:

#### Key Specifications
- **Processor**: Dual 32-bit RISC-V processors (160 MHz + 20 MHz)
- **Memory**: 512KB SRAM, 4MB Flash
- **Connectivity**: WiFi 6, Bluetooth 5.3, Zigbee, Thread
- **Size**: 21 x 17.8mm (ultra-compact)
- **Power**: Deep sleep mode (~15 µA)

#### Pin Configuration
```
GPIO Pins Used:
- GPIO 18: Relay control
- GPIO 17: Servo signal
- GPIO 15: Status LED
- GPIO 3:  WiFi RF switch control
- GPIO 14: Antenna selection
```

### Wiring Connections

```
ESP32-C6    →    Component
────────────────────────────
GPIO 18     →    Relay IN
GPIO 17     →    Servo Signal (Orange/Yellow)
GPIO 15     →    LED + (through 220Ω resistor)
3.3V        →    Relay VCC, Servo VCC (Red)
GND         →    Relay GND, Servo GND (Black/Brown), LED -
5V          →    External servo power (when available)
```

### Power Management
The system uses intelligent power management:
1. **Relay Control**: Powers servo only when needed
2. **Battery Backup**: Automatic switching to battery power
3. **Deep Sleep**: Ultra-low power consumption when idle

## 📱 Telegram Bot Setup

### Step 1: Create Your Bot with BotFather

BotFather is Telegram's official bot creation tool. To get started, message @BotFather on Telegram to register your bot and receive its authentication token.

1. **Open Telegram** and search for `@BotFather`
2. **Start a conversation** and send `/newbot`
3. **Choose a name** for your bot (e.g., "My Door Control")
4. **Choose a username** ending with "bot" (e.g., "mydoor_control_bot")
5. **Save the token** - this is your `BOT_TOKEN`

### Step 2: Get Your User ID

In your Telegram account, search for "IDBot" or open this link t.me/myidbot on your smartphone. Start a conversation with that bot and type /getid. You will get a reply back with your user ID.

1. Search for `@myidbot` in Telegram
2. Start a conversation and send `/getid`
3. **Save your User ID** - this goes in `AUTHORIZED_USERS[]`

### Step 3: Configure Bot Commands (Optional)

You can set up bot commands by going to @BotFather > /mybots > Your_Bot_Name > Edit Bot > Edit Commands.

Add these commands:
```
start - Welcome message and help
open - Open door for 10 seconds
status - Check door status
help - Show available commands
```

## ⚙️ Arduino IDE Configuration

### Step 1: Install ESP32-C6 Board Support

To install the XIAO ESP32C6 board, add the board manager URL to the preferences of your Arduino IDE.

1. **Open Arduino IDE**
2. **Go to File → Preferences**
3. **Add this URL** to "Additional Board Manager URLs":
   ```
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```
4. **Open Tools → Board → Boards Manager**
5. **Search for "esp32"** and install version **3.0.0 or later**
6. **Select Board**: Tools → Board → ESP32 Arduino → **XIAO_ESP32C6**

### Step 2: Install Required Libraries

To establish communication with the Telegram bot, we'll be using the Universal Telegram Bot Library created by Brian Lough that provides an easy interface for Telegram Bot API.

Install these libraries via **Tools → Manage Libraries**:

1. **UniversalTelegramBot** by Brian Lough
2. **ArduinoJson** by Benoît Blanchon (version 6.15.2+)
3. **ESP32Servo** (included with ESP32 core)

## 📥 Installation

### Step 1: Clone Repository
```bash
git clone https://github.com/yourusername/esp32-telegram-door-control.git
cd esp32-telegram-door-control
```

### Step 2: Configure Settings
Open the Arduino sketch and update these variables:

```cpp
// WiFi credentials
const char* ssid = "your_wifi_name";
const char* password = "your_wifi_password";

// Telegram Bot settings
#define BOT_TOKEN "your_bot_token_from_botfather"

// Authorized users (add up to 3 user IDs)
const String AUTHORIZED_USERS[] = {"123456789"}; // Your Telegram user ID
const int NUM_AUTHORIZED_USERS = 1;
```

### Step 3: Upload Code
1. **Connect ESP32-C6** via USB-C cable
2. **Select correct port** in Tools → Port
3. **Upload the sketch**

### Step 4: Monitor Serial Output
- Open **Tools → Serial Monitor**
- Set baud rate to **115200**
- Verify WiFi connection and bot initialization

## 🎮 Usage

### First Time Setup
1. **Power on** the ESP32-C6
2. **Wait for WiFi connection** (check Serial Monitor)
3. **Open Telegram** and find your bot
4. **Send `/start`** to begin

### Daily Operation
- **`/open`** - Opens door for 10 seconds
- **`/status`** - Check current door state
- **`/help`** - Show all commands

### Notifications
- All authorized users receive notifications when:
  - Door is opened by someone
  - Door closes automatically
  - System status changes

## 📋 Commands

| Command | Description | Response |
|---------|-------------|----------|
| `/start` | Initialize bot and show welcome | Welcome message with available commands |
| `/open` | Open door for 10 seconds | "🔓 Door opened! Will close in 10 seconds" |
| `/status` | Check door and system status | Door state, WiFi IP, time remaining |
| `/help` | Show help information | Complete command list and instructions |

## 🔌 Wiring Diagram

```
    ESP32-C6 XIAO
    ┌─────────────┐
    │ ┌─┐     ┌─┐ │
    │ └─┘ USB └─┘ │
    │             │
    │  18 ────────┼─── Relay IN
    │  17 ────────┼─── Servo Signal (Orange)
    │  15 ────────┼─── LED + (220Ω resistor)
    │ 3V3 ────────┼─── Relay VCC, Servo VCC (Red)
    │ GND ────────┼─── Relay GND, Servo GND (Black)
    │   5V────────┼─── External Servo Power
    └─────────────┘

    Relay Module        Servo Motor
    ┌─────────┐        ┌─────────┐
    │   VCC   │        │   VCC   │ Red
    │   GND   │        │   GND   │ Black  
    │   IN    │        │  Signal │ Orange
    │   COM   │◄──────►│         │
    │   NO    │        └─────────┘
    └─────────┘
```

## 🎯 3D Printed Parts

### Current STL Files
Compatible with **[DS3235MG Servo](https://a.co/d/hh7hMv6)**:
- Door mount bracket
- Servo housing
- Lock mechanism

### Updated Servo
**[High Torque Servo](https://a.co/d/5FDcMUa)** - *STL files need updating for this model*

> **Note**: If you choose the updated servo, you'll need to modify the STL files or design new mounts.

## 🔧 Troubleshooting

### WiFi Connection Issues
```cpp
// Check Serial Monitor for these messages:
"Connecting to WiFi..." - Normal startup
"WiFi connected!" - Success
"Failed to connect!" - Check credentials
```

### Bot Not Responding
1. **Verify bot token** in code
2. **Check user ID** in AUTHORIZED_USERS
3. **Confirm internet connection**
4. **Test with `/start` command**

### Servo Not Moving
1. **Check relay clicking sound**
2. **Verify servo connections**
3. **Test servo power supply**
4. **Ensure relay is working**

### Authorization Failures
```
"❌ Unauthorized access" - User ID not in authorized list
```

### ESP32-C6 Specific Issues

When you encounter problems with ESP32C6, you can try to put XIAO into BootLoader mode, which can solve most of the problems of unrecognized devices and failed uploads.

**BootLoader Mode**:
1. Press and hold **BOOT** button
2. Connect USB cable while holding BOOT
3. Release BOOT button
4. Upload code

## 🔒 Security Features

### Multi-User Authorization
- **User ID Verification**: Only authorized Telegram user IDs can control the door
- **Session Management**: Each command is verified before execution
- **Audit Trail**: All access attempts are logged to Serial Monitor

### Network Security
- **WPA2/WPA3 WiFi**: Secure wireless connection
- **HTTPS**: All Telegram API calls use encrypted connection
- **Token Protection**: Bot token should be kept secret

### Physical Security
- **Auto-lock**: Door automatically closes after 10 seconds
- **Power Failure Safe**: Battery backup maintains security
- **Manual Override**: Physical reset button available

## 🔋 Power Management

### Battery Configuration
The XIAO ESP32C6 features a built-in power management chip, allowing it to be powered independently by a battery or to charge the battery through its USB port.

```cpp
// Battery voltage monitoring
uint32_t Vbatt = 0;
for(int i = 0; i < 16; i++) {
    Vbatt += analogReadMilliVolts(A0);
}
float Vbattf = 2 * Vbatt / 16 / 1000.0; // Convert to volts
```

### Power Modes
- **Active Mode**: ~30mA (door operations)
- **Light Sleep**: ~2.5mA (waiting for commands)
- **Deep Sleep**: ~15µA (extended idle periods)

## 🤝 Contributing

We welcome contributions! Please follow these steps:

1. **Fork the repository**
2. **Create a feature branch**: `git checkout -b feature/amazing-feature`
3. **Commit changes**: `git commit -m 'Add amazing feature'`
4. **Push to branch**: `git push origin feature/amazing-feature`
5. **Open a Pull Request**

### Areas for Contribution
- STL file updates for new servo motor
- Additional security features
- Power optimization
- Mobile app interface
- Documentation improvements

## 📚 Additional Resources

### Documentation Links
- **[ESP32-C6 Getting Started Guide](https://wiki.seeedstudio.com/xiao_esp32c6_getting_started/)**
- **[Telegram Bot API Documentation](https://core.telegram.org/bots/api)**
- **[Universal Arduino Telegram Bot Library](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot)**
- **[ESP32 Arduino Core Documentation](https://docs.espressif.com/projects/arduino-esp32/en/latest/)**

### Hardware Datasheets
- [ESP32-C6 Technical Reference](https://files.seeedstudio.com/wiki/SeeedStudio-XIAO-ESP32C6/res/esp32-c6_datasheet_en.pdf)

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **Espressif Systems** for the ESP32-C6 chip
- **Seeed Studio** for the XIAO ESP32-C6 board
- **Brian Lough** for the Universal Telegram Bot Library
- **Telegram Team** for the excellent Bot API
- **Arduino Community** for the development environment

---

## 🚀 Quick Start Summary

1. **Hardware**: Connect relay to GPIO18, servo to GPIO17
2. **Software**: Install Arduino IDE + ESP32 board + libraries
3. **Telegram**: Create bot with @BotFather, get token and user ID
4. **Code**: Update WiFi credentials, bot token, and user ID
5. **Upload**: Flash code to ESP32-C6
6. **Test**: Send `/start` to your bot!

**Happy Building! 🏠🔐**
