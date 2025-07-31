/**
 * @file WiFiCreds.h
 * @brief Main header file for the WiFiCreds library
 * @author Rithik Krisna M
 * @version 1.0.3
 * @date 2025
 * 
 * This library provides a secure and modular way to manage multiple Wi-Fi credentials
 * in Arduino projects, separating sensitive data from the main application code.
 * 
 * @note Supports multiple named credential sets with automatic fallback
 * @note First credential set is always used as default
 */

#ifndef WIFICREDS_H
#define WIFICREDS_H

#include <Arduino.h>

/**
 * @struct CredentialSet
 * @brief Structure to hold a named set of Wi-Fi credentials
 * 
 * This structure contains a name identifier and the corresponding
 * SSID and password for a Wi-Fi network.
 */
struct CredentialSet {
    const char* name;    ///< Name identifier for the credential set (e.g., "home", "office")
    const char* ssid;    ///< Wi-Fi SSID
    const char* password; ///< Wi-Fi password
};

/**
 * @class WiFiCreds
 * @brief Main class for managing multiple Wi-Fi credentials
 * 
 * The WiFiCreds class provides static methods to access Wi-Fi SSID and password
 * credentials in a secure manner. Credentials are stored separately from the
 * main application code to improve security and maintainability.
 * 
 * @note This class uses static methods to avoid instantiation overhead
 * @note Credentials are defined in a separate credentials.h file
 * @note First credential set is always used as default
 * @note Invalid names automatically fall back to default set
 */
class WiFiCreds {
public:
    // ===== CORE CREDENTIAL METHODS =====
    
    /**
     * @brief Get the Wi-Fi SSID for a specific credential set
     * 
     * Returns the SSID for the specified credential set by name.
     * If the name is invalid or not found, falls back to the default (first) set.
     * 
     * @param name The name of the credential set (e.g., "home", "office"), or nullptr for default
     * @return const char* Pointer to the SSID string, or nullptr if no credentials available
     * @note The returned string is null-terminated
     * @note This method is thread-safe
     * @note Names are case-sensitive
     * @note Passing nullptr or invalid name uses the default (first) credential set
     */
    static const char* getSSID(const char* name = nullptr);
    
    /**
     * @brief Get the Wi-Fi password for a specific credential set
     * 
     * Returns the password for the specified credential set by name.
     * If the name is invalid or not found, falls back to the default (first) set.
     * 
     * @param name The name of the credential set (e.g., "home", "office"), or nullptr for default
     * @return const char* Pointer to the password string, or nullptr if no credentials available
     * @note The returned string is null-terminated
     * @note This method is thread-safe
     * @warning Handle the password securely and avoid logging it
     * @note Names are case-sensitive
     * @note Passing nullptr or invalid name uses the default (first) credential set
     */
    static const char* getPassword(const char* name = nullptr);
    
    /**
     * @brief Check if credentials for a specific set are properly configured
     * 
     * Validates that both SSID and password for the specified set are not null or empty strings.
     * If the name is invalid or not found, validates the default (first) set.
     * 
     * @param name The name of the credential set to validate, or nullptr for default
     * @return true if credentials are valid, false otherwise
     * @note Names are case-sensitive
     * @note Passing nullptr or invalid name validates the default (first) credential set
     */
    static bool isValid(const char* name = nullptr);
    
    /**
     * @brief Get the length of the SSID for a specific credential set
     * 
     * @param name The name of the credential set, or nullptr for default
     * @return size_t Length of the SSID string (excluding null terminator), or 0 if not found
     * @note Names are case-sensitive
     * @note Passing nullptr or invalid name uses the default (first) credential set
     */
    static size_t getSSIDLength(const char* name = nullptr);
    
    /**
     * @brief Get the length of the password for a specific credential set
     * 
     * @param name The name of the credential set, or nullptr for default
     * @return size_t Length of the password string (excluding null terminator), or 0 if not found
     * @note Names are case-sensitive
     * @note Passing nullptr or invalid name uses the default (first) credential set
     */
    static size_t getPasswordLength(const char* name = nullptr);

    // ===== CREDENTIAL MANAGEMENT METHODS =====
    
    /**
     * @brief Get the total number of available credential sets
     * 
     * @return size_t Number of credential sets defined in credentials.h
     * @note Returns 0 if no credentials are defined
     */
    static size_t getCredentialCount();
    
    /**
     * @brief Get the name of a credential set by index
     * 
     * @param index The index of the credential set (0-based)
     * @return const char* Pointer to the name string, or nullptr if index is invalid
     * @note Use getCredentialCount() to determine valid index range
     * @note Index 0 is always the default credential set
     */
    static const char* getCredentialName(size_t index);
    
    /**
     * @brief Check if a credential set with the given name exists
     * 
     * @param name The name of the credential set to check
     * @return true if the credential set exists, false otherwise
     * @note Names are case-sensitive
     * @note Returns false if name is nullptr
     */
    static bool hasCredential(const char* name);
    
    /**
     * @brief Get the default credential set name
     * 
     * @return const char* Pointer to the default credential set name, or nullptr if no credentials
     * @note The default is always the first credential set (index 0)
     */
    static const char* getDefaultName();

private:
    // Prevent instantiation of this class
    WiFiCreds() = delete;
    WiFiCreds(const WiFiCreds&) = delete;
    WiFiCreds& operator=(const WiFiCreds&) = delete;
    
    /**
     * @brief Find a credential set by name
     * 
     * @param name The name of the credential set to find
     * @return const CredentialSet* Pointer to the credential set, or nullptr if not found
     * @note Returns nullptr if name is nullptr
     */
    static const CredentialSet* findCredential(const char* name);
    
    /**
     * @brief Get the default (first) credential set
     * 
     * @return const CredentialSet* Pointer to the default credential set, or nullptr if none available
     */
    static const CredentialSet* getDefaultCredential();
};

#endif // WIFICREDS_H 