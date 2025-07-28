/**
 * @file WiFiCreds.cpp
 * @brief Implementation file for the WiFiCreds library
 * @author Rithik Krisna M
 * @version 1.0.0
 * @date 2024
 */

#include "WiFiCreds.h"
#include "credentials.h" // Contains actual SSID and password definitions

const char* WiFiCreds::getSSID() {
    return WIFI_SSID;
}

const char* WiFiCreds::getPassword() {
    return WIFI_PASS;
}

bool WiFiCreds::isValid() {
    const char* ssid = getSSID();
    const char* password = getPassword();
    
    // Check if pointers are valid and strings are not empty
    if (ssid == nullptr || password == nullptr) {
        return false;
    }
    
    // Check if strings have content (not just null terminators)
    if (strlen(ssid) == 0 || strlen(password) == 0) {
        return false;
    }
    
    return true;
}

size_t WiFiCreds::getSSIDLength() {
    const char* ssid = getSSID();
    return (ssid != nullptr) ? strlen(ssid) : 0;
}

size_t WiFiCreds::getPasswordLength() {
    const char* password = getPassword();
    return (password != nullptr) ? strlen(password) : 0;
} 