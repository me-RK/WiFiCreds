/**
 * @file WiFiCreds.h
 * @brief Main header file for the WiFiCreds library
 * @author Rithik Krisna M
 * @version 1.0.0
 * @date 2024
 * 
 * This library provides a secure and modular way to manage Wi-Fi credentials
 * in Arduino projects, separating sensitive data from the main application code.
 */

#ifndef WIFICREDS_H
#define WIFICREDS_H

#include <Arduino.h>

/**
 * @class WiFiCreds
 * @brief Main class for managing Wi-Fi credentials
 * 
 * The WiFiCreds class provides static methods to access Wi-Fi SSID and password
 * credentials in a secure manner. Credentials are stored separately from the
 * main application code to improve security and maintainability.
 * 
 * @note This class uses static methods to avoid instantiation overhead
 * @note Credentials are defined in a separate credentials.h file
 */
class WiFiCreds {
public:
    /**
     * @brief Get the Wi-Fi SSID
     * 
     * Returns the SSID (Service Set Identifier) for the Wi-Fi network.
     * The SSID is defined in the credentials.h file.
     * 
     * @return const char* Pointer to the SSID string
     * @note The returned string is null-terminated
     * @note This method is thread-safe
     */
    static const char* getSSID();
    
    /**
     * @brief Get the Wi-Fi password
     * 
     * Returns the password for the Wi-Fi network.
     * The password is defined in the credentials.h file.
     * 
     * @return const char* Pointer to the password string
     * @note The returned string is null-terminated
     * @note This method is thread-safe
     * @warning Handle the password securely and avoid logging it
     */
    static const char* getPassword();
    
    /**
     * @brief Check if credentials are properly configured
     * 
     * Validates that both SSID and password are not null or empty strings.
     * 
     * @return true if credentials are valid, false otherwise
     */
    static bool isValid();
    
    /**
     * @brief Get the length of the SSID
     * 
     * @return size_t Length of the SSID string (excluding null terminator)
     */
    static size_t getSSIDLength();
    
    /**
     * @brief Get the length of the password
     * 
     * @return size_t Length of the password string (excluding null terminator)
     */
    static size_t getPasswordLength();

private:
    // Prevent instantiation of this class
    WiFiCreds() = delete;
    WiFiCreds(const WiFiCreds&) = delete;
    WiFiCreds& operator=(const WiFiCreds&) = delete;
};

#endif // WIFICREDS_H 