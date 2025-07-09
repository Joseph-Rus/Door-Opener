# ESP32-C6 Telegram-Controlled Door Lock

A secure, multi-user Telegram bot system for controlling door locks remotely using an ESP32-C6 microcontroller. Features 270° servo rotation, power management, and real-time notifications.

## 🔥 Features

- **Multi-User Security**: Supports multiple authorized users with ID verification
- **270° Servo Control**: Full rotation lock mechanism with gradual movement
- **Power Management**: Intelligent relay control with servo power optimization
- **Auto-Close**: 10-second auto-close with countdown notifications
- **Real-Time Alerts**: All users notified when door is opened/closed
- **WiFi Status**: Built-in connection monitoring and recovery
- **Secure Authentication**: User ID verification prevents unauthorized access

## 📱 Bot Commands

- `/start` - Welcome message and command list
- `/open` - Open door for 10 seconds
- `/status` - Check door status and remaining time
- `/help` - Show detailed help information

## 🛠 Hardware Requirements

### Microcontroller
- **[Seeed Studio XIAO ESP32C6](https://a.co/d/8S7qjCs)** - Main controller with WiFi 6 and Bluetooth 5.3

### Servos
- **Current Working Servo**: [TowerPro MG996R](https://a.co/d/hh7hMv6) - Metal gear servo (180°)
- **Updated Servo**: [TowerPro MG995](https://a.co/d/5FDcMUa) - High torque servo (Note: STL files not yet updated for this servo)

### Electronics
- **[Relay Module](https://a.co/d/4TOVGFS)** - Controls servo power supply
- **[Jumper Wires](https://a.co/d/gJ13i0S)** - For connections

### 3D Printed Components
The following STL files are included in this repository:

- `bar_to_hold_servo.stl` - Mounting bracket for servo attachment
- `batteryholder.stl` - Battery compartment housing  
- `case.stl` - Main enclosure for electronics
- `lid.stl` - Top cover for the case
- `servo_holder.stl` - Servo mounting bracket

**Note**: Current STL files are designed for the [TowerPro MG996R servo](https://a.co/d/hh7hMv6). Files for the updated [TowerPro MG995 servo](https://a.co/d/5FDcMUa) are not yet available.

### Power Supply
- 5V/2A power adapter (for servo power)
- USB-C cable for ESP32-C6 programming
- 3.7V Li-Po battery (optional, for backup power)

## 🔌 Wiring Diagram

```
ESP32-C6 Pin Connections:
├─ GPIO 18 → Relay Control Pin
├─ GPIO 17 → Servo Signal Wire (PWM)
├─ GPIO 15 → Status LED
├─ 5V → Relay VCC
├─ GND → Relay GND & Servo GND
└─ Servo VCC → Relay Output (Switched 5V)
```

## 📚 Setup Documentation

### ESP32-C6 Board Setup
Follow the official Seeed Studio guide for setting up your ESP32-C6 development environment:
**[XIAO ESP32C6 Getting Started Guide](https://wiki.seeedstudio.com/xiao_esp32c6_getting_started/)**

### Required Arduino Libraries

1. **WiFi** - Built into ESP32 core
2. **WiFiClientSecure** - Built into ESP32 core  
3. **[UniversalTelegramBot](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot)** by Brian Lough
4. **[ArduinoJson](https://arduinojson.org/)** (version 6.15.2 or later)
5. **[ESP32Servo](https://github.com/madhephaestus/ESP32Servo)** 

Install via Arduino IDE Library Manager:
```
Sketch → Include Library → Manage Libraries
Search: "UniversalTelegramBot", "ArduinoJson", "ESP32Servo"
```

## 🤖 Telegram Bot Setup

### Step 1: Create Your Bot with BotFather

BotFather is Telegram's official bot for creating and managing other bots. Follow these steps:

1. **Open Telegram** and search for `@BotFather`
2. **Start conversation** and send `/newbot`
3. **Choose bot name** (e.g., "My Door Lock Bot")
4. **Choose username** (must end with "bot", e.g., "mydoorlock_bot")
5. **Save the token** - You'll receive something like: `1234567890:ABCdefGHIjklMNOpqrsTUVwxyz`

📖 **Official Documentation**: [Telegram Bot Introduction](https://core.telegram.org/bots)

### Step 2: Get Your User ID

To authorize users, you need their Telegram user IDs:

1. **Search for** `@myidbot` in Telegram
2. **Start conversation** and send `/getid`
3. **Note the number** - This is your unique user ID

### Step 3: Test Your Bot

1. Search for your bot's username in Telegram
2. Send `/start` to begin conversation
3. Your bot won't respond yet (we haven't programmed it!)

## ⚙️ Arduino Code Configuration

### Required Credentials

Update these values in the Arduino code:

```cpp
// WiFi Credentials
const char* ssid = "your_wifi_name";
const char* password = "your_wifi_password";

// Telegram Bot Configuration
#define BOT_TOKEN "your_bot_token_from_botfather"

// Authorized Users (Add up to 3 user IDs)
const String AUTHORIZED_USERS[] = {"your_user_id", "friend_user_id", "family_user_id"};
const int NUM_AUTHORIZED_USERS = 3; // Update this number
```

### Hardware Pin Configuration

```cpp
const int RELAY_PIN = 18;     // GPIO18 controls relay 
const int SERVO_PIN = 17;     // GPIO17 controls servo
const int LED_PIN = 15;       // User status LED
```

## 🔧 Installation Steps

### 1. 3D Printing
Print the required STL files from this repository:
- `case.stl` - Electronics enclosure
- `lid.stl` - Case cover  
- `servo_holder.stl` - Servo mounting bracket
- `bar_to_hold_servo.stl` - Servo attachment bar
- `batteryholder.stl` - Battery compartment (optional)

**Print Settings:** 0.2mm layer height, 20-30% infill, PLA/PETG material

### 2. Hardware Assembly
1. Install servo in printed servo holder
2. Connect components according to wiring diagram
3. Mount ESP32-C6 in printed case
4. Secure all connections and close case
5. Test servo rotation range (0° to 270°)

### 3. Software Setup
1. Install Arduino IDE with ESP32-C6 support
2. Install required libraries
3. Configure credentials in code
4. Upload to ESP32-C6

### 4. Testing
1. Monitor Serial output (115200 baud)
2. Verify WiFi connection
3. Test Telegram bot communication
4. Verify servo operation and auto-close
5. Test 3D printed mechanism operation

## 📁 Files in Repository

- `README.md` - This documentation
- `College_Dorm_Code.ino` - Main Arduino sketch  
- `bar_to_hold_servo.stl` - Servo mounting bar (3D print)
- `batteryholder.stl` - Battery compartment (3D print)
- `case.stl` - Electronics enclosure (3D print)
- `lid.stl` - Case cover (3D print)
- `servo_holder.stl` - Servo bracket (3D print)

## 📡 Arduino ESP32 Telegram Integration

The project uses the **Universal Telegram Bot Library** which provides:
- Easy interface for Telegram Bot API with support for multiple Arduino architectures
- Simple message handling and command processing
- Secure HTTPS communication with Telegram servers

### Key Library Features Used:
- `bot.getUpdates()` - Fetch new messages
- `bot.sendMessage()` - Send responses to users  
- `WiFiClientSecure` - Encrypted communication
- Certificate validation for security

📖 **Library Documentation**: [Universal Telegram Bot GitHub](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot)

## 🔒 Security Features

- **User ID Authentication**: Only authorized users can control the door
- **Secure Token Storage**: Bot token should be kept private
- **HTTPS Communication**: All Telegram communication is encrypted
- **Access Logging**: All access attempts logged to Serial monitor
- **Auto-Close Safety**: Door automatically closes after 10 seconds

## 🚨 Safety Considerations

⚠️ **Important Safety Notes:**
- Always test the system thoroughly before deployment
- Ensure backup physical key access
- Monitor power supply stability
- Test emergency scenarios
- Consider fire safety regulations
- Verify servo mounting security

## 🔧 Power Management

The system implements intelligent power management:

1. **Servo Attachment**: Only when needed for operation
2. **Relay Control**: Powers servo only during movement
3. **Gradual Movement**: Reduces current spikes
4. **Auto-Detachment**: Servo detached after closing to save power

## 📊 Status Monitoring

Monitor system status through:
- **Serial Monitor**: Debug information and connection status
- **LED Indicator**: Visual feedback on GPIO 15
- **Telegram Notifications**: Real-time door status updates
- **WiFi Signal**: Connection strength monitoring

## 🛠 Troubleshooting

### Common Issues:

**Bot Not Responding:**
- Check WiFi connection
- Verify bot token is correct
- Ensure user ID is authorized
- Check Serial monitor for errors

**Servo Not Moving:**
- Verify power supply (5V/2A minimum)
- Check wiring connections
- Test servo separately
- Monitor relay operation

**WiFi Connection Issues:**
- Verify SSID and password
- Check signal strength
- Try manual reset
- Monitor Serial for connection attempts

## 📋 Future Improvements

- [ ] Web interface for configuration
- [ ] Multiple door support
- [ ] Time-based access control
- [ ] Battery backup system
- [ ] Mobile app integration
- [ ] Voice control integration
- [ ] Security camera integration

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## 📄 License

This project is open source and available under the [MIT License](LICENSE).

## 🆘 Support

For technical support:
- **Hardware Issues**: Check [Seeed Studio XIAO ESP32C6 Wiki](https://wiki.seeedstudio.com/xiao_esp32c6_getting_started/)
- **Telegram Bot Help**: [Telegram Bot Documentation](https://core.telegram.org/bots)
- **Arduino ESP32**: [ESP32 Arduino Core Documentation](https://docs.espressif.com/projects/arduino-esp32/)

## ⭐ Acknowledgments

- [Brian Lough](https://github.com/witnessmenow) for the Universal Telegram Bot Library
- [Seeed Studio](https://www.seeedstudio.com/) for the XIAO ESP32C6 board
- Telegram team for the excellent Bot API
- Arduino and Espressif communities

---

**⚡ Quick Start**: Flash the code, update your credentials, and start controlling your door with Telegram in minutes!

**🔐 Security First**: Always test thoroughly and maintain physical backup access to your door.
