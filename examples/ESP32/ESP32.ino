/**
 * @file ESP32.ino
 * @brief WiFiCreds example for ESP32
 * @author Rithik Krisna M
 * @version 1.0.2
 * @date 2025
 * 
 * This example demonstrates WiFiCreds usage on ESP32.
 * The ESP32 has built-in Wi-Fi capabilities and supports both WiFi and WiFiClient libraries.
 * This example includes advanced features like WiFi scanning and power management.
 */

#include <WiFiCreds.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_sleep.h>

// ESP32 specific configuration
const int LED_PIN = 2; // Built-in LED on most ESP32 boards
const unsigned long WIFI_TIMEOUT = 30000; // 30 seconds timeout
const unsigned long SCAN_INTERVAL = 60000; // Scan for networks every minute

// Global variables
unsigned long lastScanTime = 0;
bool wifiConnected = false;

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
  
  Serial.println("=== ESP32 WiFiCreds Example ===");
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
  Serial.print("SSID Length: ");
  Serial.println(WiFiCreds::getSSIDLength());
  Serial.print("Password Length: ");
  Serial.println(WiFiCreds::getPasswordLength());
  Serial.println();
  
  // Configure WiFi
  configureWiFi();
  
  // Scan for available networks
  scanWiFiNetworks();
  
  // Connect to WiFi
  if (connectToWiFi()) {
    Serial.println("WiFi connected successfully!");
    wifiConnected = true;
    digitalWrite(LED_PIN, HIGH);
    printNetworkInfo();
  } else {
    Serial.println("WiFi connection failed!");
    blinkError();
  }
}

void loop() {
  // Check WiFi status
  if (WiFi.status() != WL_CONNECTED) {
    if (wifiConnected) {
      Serial.println("WiFi connection lost. Reconnecting...");
      wifiConnected = false;
      digitalWrite(LED_PIN, LOW);
    }
    
    if (connectToWiFi()) {
      Serial.println("Reconnected successfully!");
      wifiConnected = true;
      digitalWrite(LED_PIN, HIGH);
      printNetworkInfo();
    } else {
      Serial.println("Reconnection failed!");
      blinkError();
    }
  } else {
    // WiFi is connected
    if (!wifiConnected) {
      Serial.println("WiFi connection restored!");
      wifiConnected = true;
      digitalWrite(LED_PIN, HIGH);
    }
    
    // Blink LED slowly when connected (heartbeat)
    static unsigned long lastBlink = 0;
    if (millis() - lastBlink > 2000) { // Blink every 2 seconds
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      lastBlink = millis();
    }
  }
  
  // Periodically scan for networks
  if (millis() - lastScanTime > SCAN_INTERVAL) {
    scanWiFiNetworks();
    lastScanTime = millis();
  }
  
  delay(1000);
}

/**
 * @brief Configure WiFi settings for ESP32
 */
void configureWiFi() {
  Serial.println("Configuring WiFi...");
  
  // Set WiFi mode to station
  WiFi.mode(WIFI_STA);
  
  // Set WiFi power to maximum
  WiFi.setTxPower(WIFI_POWER_19_5dBm);
  
  // Set hostname
  WiFi.setHostname("ESP32-WiFiCreds");
  
  // Enable auto-reconnect
  WiFi.setAutoReconnect(true);
  
  Serial.println("WiFi configuration complete");
}

/**
 * @brief Connect to WiFi network
 * @return true if connection successful, false otherwise
 */
bool connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  Serial.print("Network: ");
  Serial.println(WiFiCreds::getSSID());
  
  // Start connection
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
      return false;
    }
  }
  
  Serial.println();
  return true;
}

/**
 * @brief Scan for available WiFi networks
 */
void scanWiFiNetworks() {
  Serial.println("Scanning for WiFi networks...");
  
  int n = WiFi.scanNetworks();
  
  if (n == 0) {
    Serial.println("No networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found:");
    
    for (int i = 0; i < n; ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
    
    // Check if our target network is in range
    bool targetFound = false;
    for (int i = 0; i < n; ++i) {
      if (WiFi.SSID(i) == WiFiCreds::getSSID()) {
        Serial.print("Target network found! Signal strength: ");
        Serial.print(WiFi.RSSI(i));
        Serial.println(" dBm");
        targetFound = true;
        break;
      }
    }
    
    if (!targetFound) {
      Serial.println("WARNING: Target network not found in scan!");
    }
  }
  
  Serial.println();
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
  Serial.print("Hostname: ");
  Serial.println(WiFi.getHostname());
  Serial.print("Channel: ");
  Serial.println(WiFi.channel());
  Serial.print("WiFi Power: ");
  Serial.print(WiFi.getTxPower());
  Serial.println(" dBm");
  Serial.println();
}

/**
 * @brief Get WiFi status as string
 * @return String representation of WiFi status
 */
String getWiFiStatusString() {
  switch (WiFi.status()) {
    case WL_CONNECTED:
      return "Connected";
    case WL_DISCONNECTED:
      return "Disconnected";
    case WL_CONNECT_FAILED:
      return "Connection Failed";
    case WL_NO_SSID_AVAIL:
      return "SSID Not Available";
    case WL_IDLE_STATUS:
      return "Idle";
    case WL_SCAN_COMPLETED:
      return "Scan Completed";
    default:
      return "Unknown";
  }
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

/**
 * @brief Enter deep sleep mode (for power saving)
 * @param seconds Sleep duration in seconds
 */
void enterDeepSleep(int seconds) {
  Serial.print("Entering deep sleep for ");
  Serial.print(seconds);
  Serial.println(" seconds...");
  
  // Turn off WiFi to save power
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  
  // Configure deep sleep
  esp_sleep_enable_timer_wakeup(seconds * 1000000ULL); // Convert to microseconds
  
  // Enter deep sleep
  esp_deep_sleep_start();
} 