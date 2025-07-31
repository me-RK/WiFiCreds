/**
 * @file WiFiCreds.cpp
 * @brief Implementation file for the WiFiCreds library
 * @author Rithik Krisna M
 * @version 1.0.3
 * @date 2025
 */

#include "WiFiCreds.h"
#include "credentials.h" // Contains actual SSID and password definitions

// ===== CORE CREDENTIAL METHODS =====

const char* WiFiCreds::getSSID(const char* name) {
    const CredentialSet* cred = (name != nullptr) ? findCredential(name) : getDefaultCredential();
    return (cred != nullptr) ? cred->ssid : nullptr;
}

const char* WiFiCreds::getPassword(const char* name) {
    const CredentialSet* cred = (name != nullptr) ? findCredential(name) : getDefaultCredential();
    return (cred != nullptr) ? cred->password : nullptr;
}

bool WiFiCreds::isValid(const char* name) {
    const char* ssid = getSSID(name);
    const char* password = getPassword(name);
    
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

size_t WiFiCreds::getSSIDLength(const char* name) {
    const char* ssid = getSSID(name);
    return (ssid != nullptr) ? strlen(ssid) : 0;
}

size_t WiFiCreds::getPasswordLength(const char* name) {
    const char* password = getPassword(name);
    return (password != nullptr) ? strlen(password) : 0;
}

// ===== CREDENTIAL MANAGEMENT METHODS =====

size_t WiFiCreds::getCredentialCount() {
    #ifdef CREDENTIAL_SETS
        size_t count = 0;
        while (CREDENTIAL_SETS[count].name != nullptr) {
            count++;
        }
        return count;
    #else
        return 0;
    #endif
}

const char* WiFiCreds::getCredentialName(size_t index) {
    #ifdef CREDENTIAL_SETS
        if (index < getCredentialCount()) {
            return CREDENTIAL_SETS[index].name;
        }
    #endif
    return nullptr;
}

bool WiFiCreds::hasCredential(const char* name) {
    if (name == nullptr) {
        return false;
    }
    return findCredential(name) != nullptr;
}

const char* WiFiCreds::getDefaultName() {
    return getCredentialName(0);
}

// ===== PRIVATE HELPER METHODS =====

const CredentialSet* WiFiCreds::findCredential(const char* name) {
    if (name == nullptr) {
        return nullptr;
    }
    
    #ifdef CREDENTIAL_SETS
        size_t count = getCredentialCount();
        for (size_t i = 0; i < count; i++) {
            if (strcmp(CREDENTIAL_SETS[i].name, name) == 0) {
                return &CREDENTIAL_SETS[i];
            }
        }
    #endif
    
    return nullptr;
}

const CredentialSet* WiFiCreds::getDefaultCredential() {
    #ifdef CREDENTIAL_SETS
        if (getCredentialCount() > 0) {
            return &CREDENTIAL_SETS[0];
        }
    #endif
    return nullptr;
} 