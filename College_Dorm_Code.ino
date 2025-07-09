#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

const char* ssid = "wifi_name";
const char* password = "wifi_password";

// Telegram Bot Token - GET THIS FROM BOTFATHER
#define BOT_TOKEN "token_from_bot_father"

// Authorized users - Add your 3 user IDs here
// Authorized users - Add your 3 user IDs here
const String AUTHORIZED_USERS[] = {"users_id"}; // Replace with actual user IDs

const int NUM_AUTHORIZED_USERS = 1;

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// Hardware pins
const int RELAY_PIN = 18;     // GPIO18 controls relay 
const int SERVO_PIN = 17;     // GPIO16 controls servo
const int LED_PIN = 15;       // User LED

// Servo setup
Servo ds3235;
bool servoAttached = false;
bool relayState = false;

// Servo positions
const int closedPosition = 0;
const int openPosition = 270;    // Full 270 degree rotation

// Timing for auto-close
bool servoIsOpen = false;
unsigned long servoOpenTime = 0;
const unsigned long servoOpenDuration = 10000; // 10 seconds

// Timing for Telegram
unsigned long lastTimeBotRan;
const int botRequestDelay = 1000; // Check for messages every 1 second

// Security function 
bool isAuthorizedUser(String chat_id) {
  for (int i = 0; i < NUM_AUTHORIZED_USERS; i++) {
    if (chat_id == AUTHORIZED_USERS[i]) {
      return true;
    }
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("=================================");
  Serial.println("Multi-User Telegram Servo Control");
  Serial.println("=================================");

  // Initialize hardware
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);

  // Start with door locked
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect to WiFi!");
    Serial.println("Restarting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Signal strength (RSSI): ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");

  // Configure secure client
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  // Test bot connection
  Serial.print("Retrieving bot name: ");
  if (bot.getMe()) {
    Serial.println("✓ Telegram bot connected successfully!");
  } else {
    Serial.println("✗ Failed to connect to Telegram bot");
    Serial.println("Check your BOT_TOKEN and internet connection");
  }

  Serial.println("=================================");
  Serial.println("Bot Ready! Available commands:");
  Serial.println("/start - Welcome message");
  Serial.println("/open  - Open servo for 10 seconds");
  Serial.println("/status - Check servo state");
  Serial.println("/help  - Show help");
  Serial.println("=================================");
}

void loop() {
  // Check for new Telegram messages
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

  // Auto-close door
  if (servoIsOpen && millis() - servoOpenTime >= servoOpenDuration) {
    closeDoor();
  }

  delay(10);
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    
    Serial.println("Message from " + from_name + " (ID: " + chat_id + "): " + text);
    
    // Security check - only allow authorized users
    if (!isAuthorizedUser(chat_id)) {
      bot.sendMessage(chat_id, "❌ Unauthorized access. Contact admin for permission.", "");
      Serial.println("Unauthorized access attempt from: " + from_name + " (ID: " + chat_id + ")");
      continue;
    }
    
    if (text == "/start") {
      String welcome = "🤖 *Secure Door Control Bot*\n\n";
      welcome += "Welcome " + from_name + "!\n\n";
      welcome += "Available commands:\n";
      welcome += "/open - Open door for 10 seconds\n";
      welcome += "/status - Check door status\n";
      welcome += "/help - Show this message";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
    
    else if (text == "/open") {
      if (!servoIsOpen) {
        openDoor();
        bot.sendMessage(chat_id, "🔓 Door opened by " + from_name + "! Will close automatically in 10 seconds.", "");
        
        // Notify other users
        notifyOtherUsers(chat_id, "🔓 Door opened by " + from_name);
      } else {
        unsigned long timeLeft = servoOpenDuration - (millis() - servoOpenTime);
        bot.sendMessage(chat_id, "⚠️ Door is already open! Closing in " + String(timeLeft/1000) + " seconds.", "");
      }
    }
    
    else if (text == "/status") {
      String status = "📊 *Door Status*\n\n";
      if (servoIsOpen) {
        unsigned long timeLeft = servoOpenDuration - (millis() - servoOpenTime);
        status += "🔓 Status: OPEN\n";
        status += "⏱️ Time left: " + String(timeLeft/1000) + " seconds";
      } else {
        status += "🔒 Status: CLOSED";
      }
      status += "\n🌐 WiFi: " + WiFi.localIP().toString();
      bot.sendMessage(chat_id, status, "Markdown");
    }
    
    else if (text == "/help") {
      String help = "🤖 *Secure Door Control Help*\n\n";
      help += "This bot controls a door lock remotely.\n\n";
      help += "Commands:\n";
      help += "/open - Opens the door for 10 seconds (270° rotation)\n";
      help += "/status - Shows current door state\n";
      help += "/help - Shows this help message\n\n";
      help += "The door will automatically close after 10 seconds.\n";
      help += "All authorized users will be notified when someone opens the door.";
      bot.sendMessage(chat_id, help, "Markdown");
    }
    
    else {
      bot.sendMessage(chat_id, "❌ Unknown command. Send /help for available commands.", "");
    }
  }
}

void notifyOtherUsers(String excludeChat, String message) {
  for (int i = 0; i < NUM_AUTHORIZED_USERS; i++) {
    if (AUTHORIZED_USERS[i] != excludeChat && AUTHORIZED_USERS[i] != "") {
      bot.sendMessage(AUTHORIZED_USERS[i], message, "");
    }
  }
}

// =======================================================
// == POWER MANAGEMENT FUNCTIONS WITH 270° ROTATION ==
// =======================================================

void openDoor() {
  Serial.println(">>> OPENING DOOR <<<");
  digitalWrite(LED_PIN, LOW); // Turn off LED to save a little power

  // 1. Attach the servo first, before the main power draw from the relay.
  if (!servoAttached) {
    ds3235.attach(SERVO_PIN);
    servoAttached = true;
    delay(500); // Wait for servo to initialize
  }

  // 2. Now, turn on the relay. This likely powers the servo's motor.
  digitalWrite(RELAY_PIN, HIGH);
  delay(500); // Give the power supply a moment to stabilize after the relay clicks on.

  Serial.println("Moving servo to open position (270°)...");
  
  // 3. Move the servo gradually to the open position (270°) to reduce current spikes.
  for (int pos = closedPosition; pos <= openPosition; pos += 5) { // Move in 5-degree increments
    ds3235.write(pos);
    delay(30); // Slower movement for the larger rotation
  }
  ds3235.write(openPosition); // Ensure it reaches the final position (270°)

  // 4. Set state for auto-close logic
  servoIsOpen = true;
  servoOpenTime = millis();

  digitalWrite(LED_PIN, HIGH); // Turn LED back on
  Serial.println("Door opened to 270°!");
}

void closeDoor() {
  Serial.println(">>> CLOSING DOOR <<<");

  if (servoAttached) {
    Serial.println("Moving servo to closed position (0°)...");
    
    // 1. Gradually move the servo back to the closed position.
    for (int pos = openPosition; pos >= closedPosition; pos -= 5) { // Move in 5-degree decrements
      ds3235.write(pos);
      delay(30); // Slower movement for the larger rotation
    }
    ds3235.write(closedPosition); // Ensure it's fully closed (0°)
    delay(500); // Let servo settle

    // 2. Detach the servo to stop it from drawing power to hold its position.
    ds3235.detach();
    servoAttached = false;
    delay(200); // Brief pause before cutting power
  }

  // 3. Cut power by turning off the relay.
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  servoIsOpen = false;
  Serial.println("Door closed and locked.");
  
  // Notify all users that door closed
  notifyOtherUsers("", "🔒 Door closed automatically");
}