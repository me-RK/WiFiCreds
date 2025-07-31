/**
 * @file SimpleExample.ino
 * @brief Simple example demonstrating WiFiCreds library usage
 * @author Rithik Krisna M
 * @version 1.0.3
 * @date 2025
 * 
 * This example demonstrates how to use the WiFiCreds library with multiple
 * credential sets. It shows the basic procedure for accessing credentials.
 * 
 * Purpose: This example shows the simple procedure for using multiple credentials
 * and displays the default credential set.
 * 
 * Prerequisites: 
 * - Create a credentials.h file in the WiFiCreds/src/ directory
 * - Define CREDENTIAL_SETS array with your network credentials
 * - See the example output below for the required format
 */

 #include <WiFiCreds.h>

 void setup() {
   // Initialize serial communication
   Serial.begin(115200);
   
   // Wait for serial to be ready
   while (!Serial) {
     delay(10);
   }
   
   Serial.println("=== WiFiCreds SimpleExample ===");
   Serial.println("Author: Rithik Krisna M");
   Serial.println("Version: 1.0.3");
   Serial.println();
   
   // Check if credentials are available
   size_t credentialCount = WiFiCreds::getCredentialCount();
   
   if (credentialCount == 0) {
     Serial.println("ERROR: No credential sets found!");
     Serial.println("Please create a credentials.h file with CREDENTIAL_SETS array.");
     Serial.println("See the example output below for the required format.");
     return;
   }
   
   Serial.print("✓ Found ");
   Serial.print(credentialCount);
   Serial.println(" credential set(s)");
   Serial.println();
   
   // Display default credentials
   displayDefaultCredentials();
   
   // Show usage procedure
   showUsageProcedure();
   
   Serial.println("Example completed successfully!");
 }
 
 void loop() {
   // This example only runs once in setup()
   // No continuous operation needed
 }
 
 /**
  * @brief Display default credentials (first set)
  */
 void displayDefaultCredentials() {
   Serial.println("Default Credentials (First Set):");
   Serial.println("=================================");
   
   const char* defaultName = WiFiCreds::getDefaultName();
   if (defaultName != nullptr) {
     Serial.print("Name: ");
     Serial.println(defaultName);
   }
   
   Serial.print("SSID: ");
   Serial.println(WiFiCreds::getSSID());
   
   Serial.print("Password: ");
   Serial.println(WiFiCreds::getPassword());
   
   Serial.print("Valid: ");
   Serial.println(WiFiCreds::isValid() ? "Yes" : "No");
   
   Serial.println();
 }
 
 /**
  * @brief Show the procedure for using the library
  */
 void showUsageProcedure() {
   Serial.println("Usage Procedure:");
   Serial.println("================");
   
   Serial.println("1. Use default credentials (first set):");
   Serial.println("   WiFi.begin(WiFiCreds::getSSID(), WiFiCreds::getPassword());");
   Serial.println();
   
   Serial.println("2. Use specific credential set:");
   Serial.println("   WiFi.begin(WiFiCreds::getSSID(\"home\"), WiFiCreds::getPassword(\"home\"));");
   Serial.println();
   
   Serial.println("3. Invalid names automatically fall back to default:");
   Serial.println("   WiFi.begin(WiFiCreds::getSSID(\"invalid\"), WiFiCreds::getPassword(\"invalid\"));");
   Serial.println("   // This uses the default (first) credential set");
   Serial.println();
   
   Serial.println("4. Check if credentials are valid:");
   Serial.println("   if (WiFiCreds::isValid()) {");
   Serial.println("     // Default credentials are valid");
   Serial.println("   }");
   Serial.println();
   
   Serial.println("5. Get all available credential names:");
   Serial.println("   size_t count = WiFiCreds::getCredentialCount();");
   Serial.println("   for (size_t i = 0; i < count; i++) {");
   Serial.println("     Serial.println(WiFiCreds::getCredentialName(i));");
   Serial.println("   }");
   Serial.println();
   
   // Show credentials.h file example
   showCredentialsFileExample();
 }
 
 /**
  * @brief Show example of credentials.h file structure
  */
 void showCredentialsFileExample() {
   Serial.println("Credentials.h File Example:");
   Serial.println("============================");
   Serial.println("Create a file named 'credentials.h' in the WiFiCreds/src/ directory:");
   Serial.println();
   Serial.println("#ifndef CREDENTIALS_H");
   Serial.println("#define CREDENTIALS_H");
   Serial.println();
   Serial.println("// Multiple credential sets");
   Serial.println("const CredentialSet CREDENTIAL_SETS[] = {");
   Serial.println("    // First set is always the default");
   Serial.println("    {");
   Serial.println("        .name = \"home\",");
   Serial.println("        .ssid = \"MyHomeWiFi\",");
   Serial.println("        .password = \"HomePassword123\"");
   Serial.println("    },");
   Serial.println("    {");
   Serial.println("        .name = \"office\",");
   Serial.println("        .ssid = \"OfficeNetwork\",");
   Serial.println("        .password = \"OfficePassword456\"");
   Serial.println("    },");
   Serial.println("    {");
   Serial.println("        .name = \"guest\",");
   Serial.println("        .ssid = \"GuestWiFi\",");
   Serial.println("        .password = \"GuestPassword789\"");
   Serial.println("    },");
   Serial.println("    // Terminator entry - must be last!");
   Serial.println("    {");
   Serial.println("        .name = nullptr,");
   Serial.println("        .ssid = nullptr,");
   Serial.println("        .password = nullptr");
   Serial.println("    }");
   Serial.println("};");
   Serial.println();
   Serial.println("#endif");
   Serial.println();
   Serial.println("IMPORTANT:");
   Serial.println("- Place credentials.h in WiFiCreds/src/ directory");
   Serial.println("- Never commit credentials.h to version control");
   Serial.println("- Add credentials.h to your .gitignore file");
   Serial.println("- First credential set is always the default");
   Serial.println();
 } 