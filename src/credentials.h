/**
 * @file credentials.h
 * @brief Wi-Fi credentials configuration file
 * @author Rithik Krisna M
 * @version 1.0.4
 * @date 2025
 * 
 * This file contains your Wi-Fi credentials for the WiFiCreds library.
 * 
 * IMPORTANT: Never commit this file to version control!
 * Add credentials.h to your .gitignore file.
 * 
 * NOTE: The first credential set is always used as the default.
 */

#ifndef CREDENTIALS_H
#define CREDENTIALS_H

// Multiple credential sets
const CredentialSet CREDENTIAL_SETS[] = {
    // First set is always the default
    {
        .name = "home",
        .ssid = "MyHomeWiFi",
        .password = "HomePassword123"
    },
    {
        .name = "office",
        .ssid = "OfficeNetwork",
        .password = "OfficePassword456"
    },
    {
        .name = "guest",
        .ssid = "GuestWiFi",
        .password = "GuestPassword789"
    },
    {
        .name = "mobile",
        .ssid = "MyPhoneHotspot",
        .password = "MobilePassword"
    },
    // Terminator entry - must be last!
    {
        .name = nullptr,
        .ssid = nullptr,
        .password = nullptr
    }
};

#endif // CREDENTIALS_H 
