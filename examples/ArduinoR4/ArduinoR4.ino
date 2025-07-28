/**
 * @file ArduinoR4.ino
 * @brief WiFiCreds example for Arduino R4 WiFi
 * @author Rithik Krisna M
 * @version 1.0.0
 * @date 2024
 * 
 * This example demonstrates WiFiCreds usage on Arduino R4 WiFi.
 * The Arduino R4 WiFi has built-in Wi-Fi capabilities and uses the WiFi library.
 * This example includes R4-specific features like LED indicators and system information.
 * 
 * Note: This example is designed for Arduino R4 WiFi. For Arduino R4 Minima,
 * you would need to use an external WiFi module like ESP8266-01.
 */

#include <WiFiCreds.h>
#include <WiFi.h>
#include <RTC.h>

// Arduino R4 specific configuration
const int LED_BUILTIN_R4 = 13; // Built-in LED on Arduino R4
const int LED_RED = 22;        // Red LED on R4 WiFi
const int LED_GREEN = 23;      // Green LED on R4 WiFi
const int LED_BLUE = 24;       // Blue LED on R4 WiFi
const unsigned long WIFI_TIMEOUT = 30000; // 30 seconds timeout
const unsigned long STATUS_INTERVAL = 10000; // Print status every 10 seconds

// Global variables
unsigned long lastStatusCheck = 0;
bool wifiConnected = false;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Initialize LEDs
  pinMode(LED_BUILTIN_R4, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  
  // Turn off all LEDs initially
  digitalWrite(LED_BUILTIN_R4, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
  
  // Wait for serial to be ready
  while (!Serial) {
    delay(10);
  }
  
  Serial.println("=== Arduino R4 WiFi WiFiCreds Example ===");
  Serial.println();
  
  // Print R4 system information
  printSystemInfo();
  
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
  
  // Connect to WiFi
  if (connectToWiFi()) {
    Serial.println("WiFi connected successfully!");
    wifiConnected = true;
    setConnectedLED();
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
      setDisconnectedLED();
    }
    
    if (connectToWiFi()) {
      Serial.println("Reconnected successfully!");
      wifiConnected = true;
      setConnectedLED();
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
      setConnectedLED();
    }
    
    // Blink green LED slowly when connected (heartbeat)
    static unsigned long lastBlink = 0;
    if (millis() - lastBlink > 2000) { // Blink every 2 seconds
      digitalWrite(LED_GREEN, !digitalRead(LED_GREEN));
      lastBlink = millis();
    }
  }
  
  // Periodically check and display status
  if (millis() - lastStatusCheck > STATUS_INTERVAL) {
    checkConnectionStatus();
    lastStatusCheck = millis();
  }
  
  delay(1000);
}

/**
 * @brief Configure WiFi settings for Arduino R4
 */
void configureWiFi() {
  Serial.println("Configuring WiFi...");
  
  // Set WiFi mode to station
  WiFi.mode(WIFI_STA);
  
  // Set hostname
  WiFi.setHostname("ArduinoR4-WiFiCreds");
  
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
    
    // Blink blue LED during connection attempt
    digitalWrite(LED_BLUE, !digitalRead(LED_BLUE));
    
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
  Serial.println();
}

/**
 * @brief Print Arduino R4 system information
 */
void printSystemInfo() {
  Serial.println("Arduino R4 System Information:");
  Serial.println("==============================");
  Serial.print("Board: ");
  Serial.println(ARDUINO_BOARD);
  Serial.print("Version: ");
  Serial.println(ARDUINO);
  Serial.print("CPU Frequency: ");
  Serial.print(F_CPU / 1000000);
  Serial.println(" MHz");
  Serial.print("Flash Size: ");
  Serial.print(FLASHEND + 1);
  Serial.println(" bytes");
  Serial.print("SRAM Size: ");
  Serial.print(RAMEND + 1);
  Serial.println(" bytes");
  Serial.println();
}

/**
 * @brief Check and display connection status
 */
void checkConnectionStatus() {
  wl_status_t status = WiFi.status();
  
  Serial.print("Connection Status: ");
  
  switch (status) {
    case WL_CONNECTED:
      Serial.println("Connected");
      Serial.print("  IP: ");
      Serial.println(WiFi.localIP());
      Serial.print("  RSSI: ");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm");
      break;
      
    case WL_DISCONNECTED:
      Serial.println("Disconnected");
      break;
      
    case WL_CONNECT_FAILED:
      Serial.println("Connection Failed");
      break;
      
    case WL_NO_SSID_AVAIL:
      Serial.println("SSID Not Available");
      break;
      
    case WL_IDLE_STATUS:
      Serial.println("Idle");
      break;
      
    case WL_SCAN_COMPLETED:
      Serial.println("Scan Completed");
      break;
      
    default:
      Serial.print("Unknown Status: ");
      Serial.println(status);
      break;
  }
  Serial.println();
}

/**
 * @brief Set LED state for connected status
 */
void setConnectedLED() {
  digitalWrite(LED_BUILTIN_R4, HIGH);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, LOW);
}

/**
 * @brief Set LED state for disconnected status
 */
void setDisconnectedLED() {
  digitalWrite(LED_BUILTIN_R4, LOW);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
}

/**
 * @brief Blink LEDs rapidly to indicate error
 */
void blinkError() {
  for (int i = 0; i < 10; i++) {
    // Blink red LED for error
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_BUILTIN_R4, HIGH);
    delay(200);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_BUILTIN_R4, LOW);
    delay(200);
  }
}

/**
 * @brief Get free memory
 * @return Free memory in bytes
 */
int getFreeMemory() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

/**
 * @brief Print memory information
 */
void printMemoryInfo() {
  Serial.println("Memory Information:");
  Serial.println("===================");
  Serial.print("Free Memory: ");
  Serial.print(getFreeMemory());
  Serial.println(" bytes");
  Serial.print("Total SRAM: ");
  Serial.print(RAMEND + 1);
  Serial.println(" bytes");
  Serial.println();
} 