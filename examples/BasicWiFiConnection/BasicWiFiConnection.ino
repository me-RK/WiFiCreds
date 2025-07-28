/**
 * @file BasicWiFiConnection.ino
 * @brief Basic Wi-Fi connection example using WiFiCreds library
 * @author Rithik Krisna M
 * @version 1.0.0
 * @date 2024
 * 
 * This example demonstrates the most basic usage of the WiFiCreds library
 * to connect to a Wi-Fi network.
 */

#include <WiFiCreds.h>
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("=== Basic WiFi Connection Example ===");
  Serial.println();
  
  // Validate credentials
  if (!WiFiCreds::isValid()) {
    Serial.println("ERROR: Invalid Wi-Fi credentials!");
    Serial.println("Please check your credentials.h file.");
    return;
  }
  
  Serial.println("Credentials validated successfully.");
  Serial.print("Connecting to: ");
  Serial.println(WiFiCreds::getSSID());
  
  // Connect to Wi-Fi
  WiFi.begin(WiFiCreds::getSSID(), WiFiCreds::getPassword());
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("Wi-Fi connected successfully!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check connection status every 5 seconds
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi connection lost. Reconnecting...");
    WiFi.reconnect();
  }
  
  delay(5000);
} 