/**
 * @file WiFiCredsDemo.ino
 * @brief Comprehensive example demonstrating WiFiCreds library usage
 * @author Rithik Krisna M
 * @version 1.0.0
 * @date 2024
 * 
 * This example demonstrates:
 * - Basic Wi-Fi connection using WiFiCreds
 * - Credential validation
 * - Error handling
 * - Network information display
 * - Connection status monitoring
 */

#include <WiFiCreds.h>
#include <WiFi.h>

// Configuration
const unsigned long WIFI_TIMEOUT = 30000; // 30 seconds timeout
const unsigned long STATUS_INTERVAL = 10000; // Print status every 10 seconds

// Global variables
unsigned long lastStatusCheck = 0;
unsigned long connectionStartTime = 0;
bool connectionAttempted = false;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("=== WiFiCreds Demo ===");
  Serial.println("Author: Rithik Krisna M");
  Serial.println("Version: 1.0.0");
  Serial.println();
  
  // Display library information
  printLibraryInfo();
  
  // Validate credentials before attempting connection
  if (!validateCredentials()) {
    Serial.println("Setup failed due to invalid credentials.");
    return;
  }
  
  // Attempt Wi-Fi connection
  if (connectToWiFi()) {
    Serial.println("Wi-Fi connection successful!");
    printNetworkInfo();
  } else {
    Serial.println("Wi-Fi connection failed!");
  }
  
  Serial.println("Setup complete. Entering main loop...");
  Serial.println();
}

void loop() {
  // Monitor connection status periodically
  if (millis() - lastStatusCheck >= STATUS_INTERVAL) {
    checkConnectionStatus();
    lastStatusCheck = millis();
  }
  
  // Handle serial commands
  if (Serial.available()) {
    handleSerialCommands();
  }
  
  // Small delay to prevent watchdog issues
  delay(100);
}

/**
 * @brief Print library information and available methods
 */
void printLibraryInfo() {
  Serial.println("Library Information:");
  Serial.println("===================");
  Serial.println("WiFiCreds Library v1.0.0");
  Serial.println("Available methods:");
  Serial.println("  - WiFiCreds::getSSID()");
  Serial.println("  - WiFiCreds::getPassword()");
  Serial.println("  - WiFiCreds::isValid()");
  Serial.println("  - WiFiCreds::getSSIDLength()");
  Serial.println("  - WiFiCreds::getPasswordLength()");
  Serial.println();
}

/**
 * @brief Validate Wi-Fi credentials
 * @return true if credentials are valid, false otherwise
 */
bool validateCredentials() {
  Serial.println("Validating Wi-Fi credentials...");
  
  if (!WiFiCreds::isValid()) {
    Serial.println("ERROR: Invalid Wi-Fi credentials!");
    Serial.println("Please check your credentials.h file.");
    return false;
  }
  
  Serial.println("✓ Credentials validation passed");
  Serial.print("  SSID: ");
  Serial.println(WiFiCreds::getSSID());
  Serial.print("  SSID Length: ");
  Serial.println(WiFiCreds::getSSIDLength());
  Serial.print("  Password Length: ");
  Serial.println(WiFiCreds::getPasswordLength());
  Serial.println();
  
  return true;
}

/**
 * @brief Connect to Wi-Fi network
 * @return true if connection successful, false otherwise
 */
bool connectToWiFi() {
  Serial.println("Connecting to Wi-Fi...");
  Serial.print("Network: ");
  Serial.println(WiFiCreds::getSSID());
  
  // Start connection attempt
  WiFi.begin(WiFiCreds::getSSID(), WiFiCreds::getPassword());
  connectionStartTime = millis();
  connectionAttempted = true;
  
  // Wait for connection with timeout
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
    // Check for timeout
    if (millis() - connectionStartTime > WIFI_TIMEOUT) {
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
  Serial.println();
}

/**
 * @brief Check and display connection status
 */
void checkConnectionStatus() {
  if (!connectionAttempted) {
    return;
  }
  
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
 * @brief Handle serial commands from user
 */
void handleSerialCommands() {
  String command = Serial.readStringUntil('\n');
  command.trim();
  command.toLowerCase();
  
  if (command == "help" || command == "h") {
    printHelp();
  } else if (command == "status" || command == "s") {
    checkConnectionStatus();
  } else if (command == "info" || command == "i") {
    printNetworkInfo();
  } else if (command == "reconnect" || command == "r") {
    Serial.println("Reconnecting to Wi-Fi...");
    WiFi.disconnect();
    delay(1000);
    if (connectToWiFi()) {
      Serial.println("Reconnection successful!");
      printNetworkInfo();
    } else {
      Serial.println("Reconnection failed!");
    }
  } else if (command == "credentials" || command == "c") {
    printCredentialInfo();
  } else {
    Serial.print("Unknown command: ");
    Serial.println(command);
    Serial.println("Type 'help' for available commands.");
  }
}

/**
 * @brief Print help information
 */
void printHelp() {
  Serial.println("Available Commands:");
  Serial.println("===================");
  Serial.println("help (h)      - Show this help message");
  Serial.println("status (s)    - Show connection status");
  Serial.println("info (i)      - Show network information");
  Serial.println("reconnect (r) - Reconnect to Wi-Fi");
  Serial.println("credentials (c) - Show credential information");
  Serial.println();
}

/**
 * @brief Print credential information (without revealing password)
 */
void printCredentialInfo() {
  Serial.println("Credential Information:");
  Serial.println("======================");
  Serial.print("SSID: ");
  Serial.println(WiFiCreds::getSSID());
  Serial.print("SSID Length: ");
  Serial.println(WiFiCreds::getSSIDLength());
  Serial.print("Password Length: ");
  Serial.println(WiFiCreds::getPasswordLength());
  Serial.print("Credentials Valid: ");
  Serial.println(WiFiCreds::isValid() ? "Yes" : "No");
  Serial.println();
} 