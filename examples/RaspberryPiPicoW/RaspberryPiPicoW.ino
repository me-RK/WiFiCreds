/**
 * @file RaspberryPiPicoW.ino
 * @brief WiFiCreds example for Raspberry Pi Pico W
 * @author Rithik Krisna M
 * @version 1.0.3
 * @date 2025
 * 
 * This example demonstrates WiFiCreds usage on Raspberry Pi Pico W.
 * The Pico W has built-in Wi-Fi capabilities and uses the WiFi library.
 */

#include <WiFiCreds.h>
#include <WiFi.h>

// Pico W specific configuration
const int LED_PIN = 25; // Built-in LED on Pico W
const unsigned long WIFI_TIMEOUT = 30000; // 30 seconds timeout

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Initialize built-in LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Wait for serial to be ready
  while (!Serial) {
    delay(10);
  }
  
  Serial.println("=== Raspberry Pi Pico W WiFiCreds Example ===");
  Serial.println();
  
  // Validate credentials
  if (!WiFiCreds::isValid()) {
    Serial.println("ERROR: Invalid Wi-Fi credentials!");
    Serial.println("Please check your credentials.h file.");
    blinkError();
    return;
  }
  
  Serial.println("Credentials validated successfully.");
  Serial.print("Connecting to: ");
  Serial.println(WiFiCreds::getSSID());
  
  // Connect to Wi-Fi
  WiFi.begin(WiFiCreds::getSSID(), WiFiCreds::getPassword());
  
  // Wait for connection with timeout
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
    // Blink LED during connection attempt
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    
    // Check for timeout
    if (millis() - startTime > WIFI_TIMEOUT) {
      Serial.println();
      Serial.println("ERROR: Connection timeout!");
      blinkError();
      return;
    }
  }
  
  Serial.println();
  Serial.println("Wi-Fi connected successfully!");
  
  // Turn on LED to indicate success
  digitalWrite(LED_PIN, HIGH);
  
  // Print network information
  printNetworkInfo();
}

void loop() {
  // Check connection status
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi connection lost. Reconnecting...");
    digitalWrite(LED_PIN, LOW);
    WiFi.reconnect();
    
    // Wait for reconnection
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_TIMEOUT) {
      delay(500);
      Serial.print(".");
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.println("Reconnected successfully!");
      digitalWrite(LED_PIN, HIGH);
    } else {
      Serial.println();
      Serial.println("Reconnection failed!");
      blinkError();
    }
  }
  
  // Blink LED slowly when connected (heartbeat)
  if (WiFi.status() == WL_CONNECTED) {
    static unsigned long lastBlink = 0;
    if (millis() - lastBlink > 2000) { // Blink every 2 seconds
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      lastBlink = millis();
    }
  }
  
  delay(1000);
}

/**
 * @brief Print current network information
 */
void printNetworkInfo() {
  Serial.println("Network Information:");
  Serial.println("===================");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("DNS Server: ");
  Serial.println(WiFi.dnsIP());
  Serial.print("Signal Strength (RSSI): ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.println();
}

/**
 * @brief Blink LED rapidly to indicate error
 */
void blinkError() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
} 