/**
 * @file SimpleExample.ino
 * @brief Simple example demonstrating WiFiCreds library usage
 * @author Rithik Krisna M
 * @version 1.0.0
 * @date 2025
 * 
 * This example demonstrates the most basic usage of the WiFiCreds library.
 * It shows how to access and display stored Wi-Fi credentials without
 * actually connecting to a network.
 * 
 * Purpose: This example is perfect for testing if your credentials.h file
 * is properly configured and the library is working correctly.
 */

#include <WiFiCreds.h>

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Wait for serial to be ready
  while (!Serial) {
    delay(10);
  }
  
  Serial.println("=== WiFiCreds Simple Example ===");
  Serial.println("Author: Rithik Krisna M");
  Serial.println("Version: 1.0.0");
  Serial.println();
  
  // Validate credentials before accessing them
  if (!WiFiCreds::isValid()) {
    Serial.println("ERROR: Invalid Wi-Fi credentials!");
    Serial.println("Please check your credentials.h file.");
    Serial.println("Make sure WIFI_SSID and WIFI_PASS are properly defined.");
    return;
  }
  
  Serial.println("✓ Credentials validated successfully");
  Serial.println();
  
  // Display credential information
  displayCredentials();
  
  Serial.println("Example completed successfully!");
  Serial.println("You can now use these credentials in your WiFi connection code.");
}

void loop() {
  // This example only runs once in setup()
  // No continuous operation needed
}

/**
 * @brief Display stored Wi-Fi credentials
 */
void displayCredentials() {
  Serial.println("Stored Wi-Fi Credentials:");
  Serial.println("==========================");
  
  Serial.print("SSID: ");
  Serial.println(WiFiCreds::getSSID());
  
  Serial.print("Password: ");
  Serial.println(WiFiCreds::getPassword());
  
  Serial.print("SSID Length: ");
  Serial.println(WiFiCreds::getSSIDLength());
  
  Serial.print("Password Length: ");
  Serial.println(WiFiCreds::getPasswordLength());
  
  Serial.println();
} 