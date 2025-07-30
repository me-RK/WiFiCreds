/**
 * @file ESP8266.ino
 * @brief WiFiCreds example for ESP8266
 * @author Rithik Krisna M
 * @version 1.0.2
 * @date 2025
 * 
 * This example demonstrates WiFiCreds usage on ESP8266.
 * The ESP8266 has built-in Wi-Fi capabilities and uses the ESP8266WiFi library.
 * This example includes ESP8266-specific features like WiFi scanning and power management.
 */

#include <WiFiCreds.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

// ESP8266 specific configuration
const int LED_PIN = 2; // Built-in LED on most ESP8266 boards (inverted logic)
const unsigned long WIFI_TIMEOUT = 30000; // 30 seconds timeout
const unsigned long SCAN_INTERVAL = 60000; // Scan for networks every minute

// Create WiFiMulti object for multiple network support
ESP8266WiFiMulti wifiMulti;

// Global variables
unsigned long lastScanTime = 0;
bool wifiConnected = false;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Initialize built-in LED (inverted logic on ESP8266)
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // LED off initially
  
  // Wait for serial to be ready
  while (!Serial) {
    delay(10);
  }
  
  Serial.println("=== ESP8266 WiFiCreds Example ===");
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
    digitalWrite(LED_PIN, LOW); // LED on (inverted logic)
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
      digitalWrite(LED_PIN, HIGH); // LED off (inverted logic)
    }
    
    if (connectToWiFi()) {
      Serial.println("Reconnected successfully!");
      wifiConnected = true;
      digitalWrite(LED_PIN, LOW); // LED on (inverted logic)
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
      digitalWrite(LED_PIN, LOW); // LED on (inverted logic)
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
 * @brief Configure WiFi settings for ESP8266
 */
void configureWiFi() {
  Serial.println("Configuring WiFi...");
  
  // Set WiFi mode to station
  WiFi.mode(WIFI_STA);
  
  // Set hostname
  WiFi.hostname("ESP8266-WiFiCreds");
  
  // Enable auto-reconnect
  WiFi.setAutoReconnect(true);
  
  // Set sleep mode to none for better performance
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  
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
    
    // Blink LED during connection attempt (inverted logic)
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
      Serial.print(" Channel: ");
      Serial.print(WiFi.channel(i));
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
    
    // Check if our target network is in range
    bool targetFound = false;
    for (int i = 0; i < n; ++i) {
      if (WiFi.SSID(i) == WiFiCreds::getSSID()) {
        Serial.print("Target network found! Signal strength: ");
        Serial.print(WiFi.RSSI(i));
        Serial.print(" dBm, Channel: ");
        Serial.println(WiFi.channel(i));
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
  Serial.println(WiFi.hostname());
  Serial.print("Channel: ");
  Serial.println(WiFi.channel());
  Serial.print("WiFi Mode: ");
  Serial.println(WiFi.getMode());
  Serial.print("Sleep Mode: ");
  Serial.println(WiFi.getSleepMode());
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
 * @brief Blink LED rapidly to indicate error (inverted logic)
 */
void blinkError() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_PIN, LOW);  // LED on
    delay(200);
    digitalWrite(LED_PIN, HIGH); // LED off
    delay(200);
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
  
  // Enter deep sleep
  ESP.deepSleep(seconds * 1000000ULL); // Convert to microseconds
}

/**
 * @brief Get free heap memory
 * @return Free heap memory in bytes
 */
uint32_t getFreeHeap() {
  return ESP.getFreeHeap();
}

/**
 * @brief Print system information
 */
void printSystemInfo() {
  Serial.println("System Information:");
  Serial.println("===================");
  Serial.print("Free Heap: ");
  Serial.print(getFreeHeap());
  Serial.println(" bytes");
  Serial.print("Chip ID: ");
  Serial.println(ESP.getChipId());
  Serial.print("Flash Chip ID: ");
  Serial.println(ESP.getFlashChipId());
  Serial.print("Flash Chip Size: ");
  Serial.print(ESP.getFlashChipSize());
  Serial.println(" bytes");
  Serial.print("SDK Version: ");
  Serial.println(ESP.getSdkVersion());
  Serial.print("Core Version: ");
  Serial.println(ESP.getCoreVersion());
  Serial.print("Boot Version: ");
  Serial.println(ESP.getBootVersion());
  Serial.print("Boot Mode: ");
  Serial.println(ESP.getBootMode());
  Serial.println();
} 