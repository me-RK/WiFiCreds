# WiFiCreds Library

[![Version](https://img.shields.io/badge/version-1.0.2-blue.svg)](https://github.com/me-RK/WiFiCreds)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Arduino-orange.svg)](https://www.arduino.cc/)
[![ESP32](https://img.shields.io/badge/ESP32-Supported-brightgreen.svg)](https://www.espressif.com/)
[![ESP8266](https://img.shields.io/badge/ESP8266-Supported-brightgreen.svg)](https://www.espressif.com/)
[![Arduino R4 WiFi](https://img.shields.io/badge/Arduino%20R4%20WiFi-Supported-brightgreen.svg)](https://store.arduino.cc/products/arduino-uno-r4-wifi)
[![Raspberry Pi Pico W](https://img.shields.io/badge/Raspberry%20Pi%20Pico%20W-Supported-brightgreen.svg)](https://www.raspberrypi.com/products/raspberry-pi-pico/)
[![Arduino + ESP8266-01](https://img.shields.io/badge/Arduino%20%2B%20ESP8266--01-Supported-brightgreen.svg)](https://www.espressif.com/)
[![Arduino + ESP32](https://img.shields.io/badge/Arduino%20%2B%20ESP32-Supported-brightgreen.svg)](https://www.espressif.com/)

A secure and modular Arduino library for managing Wi-Fi credentials, designed to prevent accidental exposure of sensitive data when sharing code.

## 🎯 Primary Purpose

**The main objective of this library is simple: to prevent accidental exposure of Wi-Fi credentials when sharing code.**

This library provides **peace of mind** when sharing your Arduino projects. Instead of having your Wi-Fi credentials scattered throughout your code where they might be accidentally committed to version control or shared with others, this library keeps them in a separate, easily manageable file.

### How It Works

When you share your code with clients or collaborators, they have two simple options:

1. **Remove the WiFiCreds library code** and manually add their own credentials directly in their code
2. **Install the library** and create their own `credentials.h` file with their hardcoded credentials (just like you did)

This approach ensures that your sensitive network information never gets exposed, while still providing a clean, professional way to manage credentials in your projects.

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

Create a `credentials.h` file in the library's `src` folder:

```cpp
#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#define WIFI_SSID "YourNetworkName"
#define WIFI_PASS "YourSecurePassword"

#endif
```

**Important**: The `credentials.h` file must be placed in the `WiFiCreds/src/` directory, not in your project directory. This ensures the credentials are properly included by the library.

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

### Getting Started Examples
- **SimpleExample**: Basic demonstration of accessing and displaying stored credentials without connecting to WiFi
- **BasicWiFiConnection**: Simple Wi-Fi connection example
- **WiFiCredsDemo**: Comprehensive example with interactive features

### Platform-Specific Examples
- **RaspberryPiPicoW**: Example for Raspberry Pi Pico W with built-in LED indicators
- **ESP32**: Advanced ESP32 example with WiFi scanning and power management
- **ESP8266**: ESP8266-specific example with system information and deep sleep
- **ArduinoR4**: Arduino R4 WiFi example with RGB LED indicators and system information
- **ArduinoESP8266-01**: Arduino with ESP8266-01 module using SoftwareSerial communication

### SimpleExample Walkthrough

The **SimpleExample** demonstrates the most basic usage of the WiFiCreds library:

1. **Purpose**: Tests if your `credentials.h` file is properly configured
2. **Functionality**: Displays stored credentials without connecting to WiFi
3. **Use Case**: Perfect for verifying library installation and credential setup

```cpp
#include <WiFiCreds.h>

void setup() {
  Serial.begin(115200);
  
  // Validate credentials
  if (!WiFiCreds::isValid()) {
    Serial.println("ERROR: Invalid Wi-Fi credentials!");
    return;
  }
  
  // Display credentials
  Serial.print("SSID: ");
  Serial.println(WiFiCreds::getSSID());
  Serial.print("Password: ");
  Serial.println(WiFiCreds::getPassword());
}
```

This example is ideal for:
- Testing your credential configuration
- Verifying library installation
- Understanding basic API usage
- Debugging credential-related issues

## Security Best Practices

1. **Never commit credentials**: Add `credentials.h` to your `.gitignore`
2. **Use strong passwords**: Ensure your Wi-Fi password is secure
3. **Production deployment**: Consider using secure storage methods for production
4. **Regular updates**: Keep your credentials updated and secure
5. **Share safely**: When sharing code, ensure `credentials.h` is excluded

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

- **v1.0.2**: Enhanced version with improved documentation and examples
- **v1.0.0**: Initial release with basic credential management

## Support

If you encounter any issues or have questions:

1. Check the [examples](examples/) folder
2. Review the [documentation](docs/)
3. Open an [issue](https://github.com/me-RK/WiFiCreds/issues)
4. Check the [Arduino Forum](https://forum.arduino.cc/)

---

**Note**: This library is designed for educational and development purposes. For production applications, consider implementing additional security measures appropriate for your use case. 