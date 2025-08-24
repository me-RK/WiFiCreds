/**
 * @file ArduinoESP8266-01.ino
 * @brief WiFiCreds example for Arduino with ESP8266-01 module
 * @author Rithik Krisna M
 * @version 1.0.4
 * @date 2025
 * 
 * This example demonstrates WiFiCreds usage with Arduino and ESP8266-01 module.
 * The ESP8266-01 communicates with Arduino via SoftwareSerial.
 * 
 * Hardware Connections:
 * - Arduino TX (Pin 2) -> ESP8266-01 RX
 * - Arduino RX (Pin 3) -> ESP8266-01 TX
 * - Arduino 3.3V -> ESP8266-01 VCC
 * - Arduino GND -> ESP8266-01 GND
 * - Arduino Pin 4 -> ESP8266-01 CH_PD (Enable)
 */

#include <WiFiCreds.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

// ESP8266-01 configuration
const int ESP_RX = 2;  // Arduino pin connected to ESP8266 TX
const int ESP_TX = 3;  // Arduino pin connected to ESP8266 RX
const int ESP_ENABLE = 4; // Arduino pin connected to ESP8266 CH_PD
const int STATUS_LED = 13; // Built-in LED on Arduino

// Create SoftwareSerial object for ESP8266 communication
SoftwareSerial esp8266(ESP_RX, ESP_TX);

// WiFi configuration
const unsigned long WIFI_TIMEOUT = 30000; // 30 seconds timeout
const unsigned long BAUD_RATE = 115200;

void setup() {
  // Initialize Arduino serial for debugging
  Serial.begin(115200);
  
  // Initialize pins
  pinMode(ESP_ENABLE, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW);
  
  Serial.println("=== Arduino + ESP8266-01 WiFiCreds Example ===");
  Serial.println();
  
  // Validate credentials
  if (!WiFiCreds::isValid()) {
    Serial.println("ERROR: Invalid Wi-Fi credentials!");
    Serial.println("Please check your credentials.h file.");
    blinkError();
    return;
  }
  
  Serial.println("Credentials validated successfully.");
  Serial.print("SSID: ");
  Serial.println(WiFiCreds::getSSID());
  Serial.print("Password Length: ");
  Serial.println(WiFiCreds::getPasswordLength());
  Serial.println();
  
  // Initialize ESP8266
  if (!initializeESP8266()) {
    Serial.println("ERROR: Failed to initialize ESP8266!");
    blinkError();
    return;
  }
  
  // Connect to WiFi
  if (connectToWiFi()) {
    Serial.println("WiFi connected successfully!");
    digitalWrite(STATUS_LED, HIGH);
    printNetworkInfo();
  } else {
    Serial.println("WiFi connection failed!");
    blinkError();
  }
}

void loop() {
  // Check WiFi status
  String response = sendATCommand("AT+CWJAP?", 1000);
  
  if (response.indexOf("+CWJAP:") != -1) {
    // Still connected
    static unsigned long lastBlink = 0;
    if (millis() - lastBlink > 3000) { // Blink every 3 seconds
      digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
      lastBlink = millis();
    }
  } else {
    // Connection lost, try to reconnect
    Serial.println("WiFi connection lost. Reconnecting...");
    digitalWrite(STATUS_LED, LOW);
    
    if (connectToWiFi()) {
      Serial.println("Reconnected successfully!");
      digitalWrite(STATUS_LED, HIGH);
    } else {
      Serial.println("Reconnection failed!");
      blinkError();
    }
  }
  
  delay(5000);
}

/**
 * @brief Initialize ESP8266 module
 * @return true if initialization successful, false otherwise
 */
bool initializeESP8266() {
  Serial.println("Initializing ESP8266...");
  
  // Enable ESP8266
  digitalWrite(ESP_ENABLE, HIGH);
  delay(1000);
  
  // Initialize SoftwareSerial
  esp8266.begin(BAUD_RATE);
  delay(1000);
  
  // Test AT command
  String response = sendATCommand("AT", 1000);
  if (response.indexOf("OK") == -1) {
    Serial.println("ESP8266 not responding to AT commands");
    return false;
  }
  
  Serial.println("ESP8266 initialized successfully");
  
  // Set mode to station
  response = sendATCommand("AT+CWMODE=1", 1000);
  if (response.indexOf("OK") == -1) {
    Serial.println("Failed to set WiFi mode");
    return false;
  }
  
  Serial.println("WiFi mode set to station");
  return true;
}

/**
 * @brief Connect to WiFi network
 * @return true if connection successful, false otherwise
 */
bool connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  Serial.print("Network: ");
  Serial.println(WiFiCreds::getSSID());
  
  // Build AT command for WiFi connection
  String cmd = "AT+CWJAP=\"";
  cmd += WiFiCreds::getSSID();
  cmd += "\",\"";
  cmd += WiFiCreds::getPassword();
  cmd += "\"";
  
  // Send connection command
  String response = sendATCommand(cmd, WIFI_TIMEOUT);
  
  if (response.indexOf("WIFI CONNECTED") != -1 && response.indexOf("WIFI GOT IP") != -1) {
    Serial.println("WiFi connection established");
    return true;
  } else {
    Serial.println("WiFi connection failed");
    return false;
  }
}

/**
 * @brief Send AT command to ESP8266
 * @param command The AT command to send
 * @param timeout Timeout in milliseconds
 * @return Response from ESP8266
 */
String sendATCommand(String command, unsigned long timeout) {
  String response = "";
  unsigned long startTime = millis();
  
  // Clear any pending data
  while (esp8266.available()) {
    esp8266.read();
  }
  
  // Send command
  esp8266.println(command);
  
  // Wait for response
  while (millis() - startTime < timeout) {
    if (esp8266.available()) {
      char c = esp8266.read();
      response += c;
      
      // Check for end of response
      if (response.indexOf("OK") != -1 || response.indexOf("ERROR") != -1) {
        break;
      }
    }
  }
  
  return response;
}

/**
 * @brief Print current network information
 */
void printNetworkInfo() {
  Serial.println("Network Information:");
  Serial.println("===================");
  
  // Get current AP info
  String response = sendATCommand("AT+CWJAP?", 1000);
  if (response.indexOf("+CWJAP:") != -1) {
    Serial.print("Connected to: ");
    int start = response.indexOf("\"") + 1;
    int end = response.indexOf("\"", start);
    Serial.println(response.substring(start, end));
  }
  
  // Get IP address
  response = sendATCommand("AT+CIFSR", 1000);
  if (response.indexOf("STAIP") != -1) {
    Serial.print("IP Address: ");
    int start = response.indexOf("\"") + 1;
    int end = response.indexOf("\"", start);
    Serial.println(response.substring(start, end));
  }
  
  Serial.println();
}

/**
 * @brief Blink LED rapidly to indicate error
 */
void blinkError() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(STATUS_LED, HIGH);
    delay(200);
    digitalWrite(STATUS_LED, LOW);
    delay(200);
  }
} 