/**
 * @file WiFiCreds.cpp
 * @brief Implementation file for the WiFiCreds library
 * @author Rithik Krisna M
 * @version 1.0.4
 * @date 2025
 */

#include "WiFiCreds.h"
#include "credentials.h" // Contains actual SSID and password definitions
#include <string.h>     // Required for strcmp and strlen

// ===== CORE CREDENTIAL METHODS =====

const char* WiFiCreds::getSSID(const char* name) {
    const CredentialSet* cred = (name != nullptr) ? findCredential(name) : getDefaultCredential();
    
    // If named credential not found, fall back to default
    if (cred == nullptr && name != nullptr) {
        cred = getDefaultCredential();
    }
    
    return (cred != nullptr) ? cred->ssid : nullptr;
}

const char* WiFiCreds::getPassword(const char* name) {
    const CredentialSet* cred = (name != nullptr) ? findCredential(name) : getDefaultCredential();
    
    // If named credential not found, fall back to default
    if (cred == nullptr && name != nullptr) {
        cred = getDefaultCredential();
    }
    
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
    size_t count = 0;
    
    // Count entries until we hit the terminator (where name is nullptr)
    while (count < 1000 && CREDENTIAL_SETS[count].name != nullptr) { // Safety limit to prevent infinite loops
        count++;
    }
    
    return count;
}

const char* WiFiCreds::getCredentialName(size_t index) {
    size_t totalCount = getCredentialCount();
    
    if (index < totalCount) {
        return CREDENTIAL_SETS[index].name;
    }
    
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
    
    size_t count = getCredentialCount();
    for (size_t i = 0; i < count; i++) {
        if (strcmp(CREDENTIAL_SETS[i].name, name) == 0) {
            return &CREDENTIAL_SETS[i];
        }
    }
    
    return nullptr;
}

const CredentialSet* WiFiCreds::getDefaultCredential() {
    if (getCredentialCount() > 0) {
        return &CREDENTIAL_SETS[0];
    }
    return nullptr;
}