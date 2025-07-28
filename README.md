# WiFiCreds Library

A secure and modular Arduino library for managing Wi-Fi credentials, designed to separate sensitive data from your main application code.

## Features

- 🔒 **Secure Credential Management**: Keeps Wi-Fi credentials separate from main code
- 📚 **Easy Integration**: Simple static methods for accessing credentials
- 🛡️ **Validation**: Built-in credential validation
- 📖 **Well Documented**: Comprehensive Doxygen documentation
- 🔧 **Modular Design**: Easy to extend for different storage methods
- 🎯 **Production Ready**: Follows Arduino library best practices

## Platform Compatibility

The WiFiCreds library is compatible with the following platforms:

- ✅ **ESP32**: Full support with built-in WiFi capabilities
- ✅ **ESP8266**: Full support with built-in WiFi capabilities  
- ✅ **Raspberry Pi Pico W**: Full support with built-in WiFi capabilities
- ✅ **Arduino R4 WiFi**: Full support with built-in WiFi capabilities
- ✅ **Arduino + ESP8266-01**: Support via SoftwareSerial communication
- ✅ **Arduino + ESP8266**: Support via ESP8266WiFi library
- ✅ **Arduino + ESP32**: Support via WiFi library

## Installation

### Arduino Library Manager (Recommended)
1. Open Arduino IDE
2. Go to **Tools** → **Manage Libraries**
3. Search for "WiFiCreds"
4. Click **Install**

### Manual Installation
1. Download or clone this repository
2. Copy the `WiFiCreds` folder to your Arduino libraries directory:
   - **Windows**: `Documents\Arduino\libraries\`
   - **macOS**: `~/Documents/Arduino/libraries/`
   - **Linux**: `~/Arduino/libraries/`
3. Restart Arduino IDE

## Quick Start

### 1. Configure Your Credentials

Create a `credentials.h` file in your project directory:

```cpp
#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#define WIFI_SSID "YourNetworkName"
#define WIFI_PASS "YourSecurePassword"

#endif
```

### 2. Basic Usage

```cpp
#include <WiFiCreds.h>
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  
  // Validate credentials before use
  if (!WiFiCreds::isValid()) {
    Serial.println("Error: Invalid Wi-Fi credentials!");
    return;
  }
  
  // Connect to Wi-Fi
  WiFi.begin(WiFiCreds::getSSID(), WiFiCreds::getPassword());
  
  Serial.print("Connecting to ");
  Serial.println(WiFiCreds::getSSID());
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to Wi-Fi!");
}

void loop() {
  // Your main code here
}
```

## API Reference

### Static Methods

#### `getSSID()`
Returns the Wi-Fi SSID as a null-terminated string.

```cpp
const char* ssid = WiFiCreds::getSSID();
```

#### `getPassword()`
Returns the Wi-Fi password as a null-terminated string.

```cpp
const char* password = WiFiCreds::getPassword();
```

#### `isValid()`
Validates that both SSID and password are properly configured.

```cpp
if (WiFiCreds::isValid()) {
  // Credentials are valid
}
```

#### `getSSIDLength()`
Returns the length of the SSID string (excluding null terminator).

```cpp
size_t ssidLength = WiFiCreds::getSSIDLength();
```

#### `getPasswordLength()`
Returns the length of the password string (excluding null terminator).

```cpp
size_t passwordLength = WiFiCreds::getPasswordLength();
```

## Examples

The library includes several example sketches for different platforms:

### General Examples
- **BasicWiFiConnection**: Simple Wi-Fi connection example
- **WiFiCredsDemo**: Comprehensive example with interactive features
- **CredentialManager**: Advanced credential management with validation
- **APModeFallback**: Wi-Fi connection with Access Point fallback

### Platform-Specific Examples
- **RaspberryPiPicoW**: Example for Raspberry Pi Pico W with built-in LED indicators
- **ESP32**: Advanced ESP32 example with WiFi scanning and power management
- **ESP8266**: ESP8266-specific example with system information and deep sleep
- **ArduinoR4**: Arduino R4 WiFi example with RGB LED indicators and system information
- **ArduinoESP8266-01**: Arduino with ESP8266-01 module using SoftwareSerial communication

## Security Best Practices

1. **Never commit credentials**: Add `credentials.h` to your `.gitignore`
2. **Use strong passwords**: Ensure your Wi-Fi password is secure
3. **Production deployment**: Consider using secure storage methods for production
4. **Regular updates**: Keep your credentials updated and secure

## Future Enhancements

The library is designed to be extensible. Future versions may include:

- **EEPROM Storage**: Store credentials in EEPROM memory
- **SPIFFS Support**: Load credentials from SPIFFS file system
- **Secure Elements**: Integration with hardware security modules
- **Encryption**: Encrypted credential storage
- **Multiple Networks**: Support for multiple Wi-Fi networks
- **OTA Updates**: Over-the-air credential updates

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## License

This library is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Author

**Rithik Krisna M** ([@me-RK](https://github.com/me-RK))

## Version History

- **v1.0.0**: Initial release with basic credential management

## Support

If you encounter any issues or have questions:

1. Check the [examples](examples/) folder
2. Review the [documentation](docs/)
3. Open an [issue](https://github.com/me-RK/WiFiCreds/issues)
4. Check the [Arduino Forum](https://forum.arduino.cc/)

---

**Note**: This library is designed for educational and development purposes. For production applications, consider implementing additional security measures appropriate for your use case. 