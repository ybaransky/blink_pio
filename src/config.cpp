#include <ArduinoJson.h>
#include <FS.h>
#include <LittleFS.h>
#include <SPI.h>

#include "config.h"
#include "utils.h"

#define FILESYSTEM  LittleFS
#define FILE_WRITE  "w"
#define FILE_READ   "r"

void WifiParameters::init(void) {
  strcpy(hostname,"askyurij.com");
  strcpy(password,"leslie1a");
  channel = 0;
}

void WifiParameters::print(void) {
  PVL(hostname);
  PVL(password);
  PVL(channel);
}

// Our configuration structure.
//
// Never use a JsonDocument to store the configuration!
// A JsonDocument is *not* a permanent storage; it's only a temporary storage
// used during the serialization phase. See:
// https://arduinojson.org/v6/faq/why-must-i-create-a-separate-config-object/

void Config::init(const char* filename) {
  strcpy(this->filename, filename);
  wifiParameters.init();

  // Initialize SD library
  while (!FILESYSTEM.begin()) {
    Serial.println(F("Failed to initialize SD library"));
    delay(1000);
  }
}

// Loads the configuration from a file
void Config::load(void) {
  // Open file for reading
  File file = FILESYSTEM.open(filename, FILE_READ);

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<512> doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  // Copy values from the JsonDocument to the Config
  strlcpy(wifiParameters.hostname, doc["hostname"] | "empty.com", sizeof(wifiParameters.hostname));
  strlcpy(wifiParameters.password, doc["password"] | "deadbeef",  sizeof(wifiParameters.password));
  data = doc["data"] | 1234;

  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();
}

// Saves the configuration to a file
void Config::save(void) {
  // Delete existing file, otherwise the configuration is appended to the file
  FILESYSTEM.remove(filename);

  // Open file for writing
  File file = FILESYSTEM.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<256> doc;

  // Set the values in the document
  doc["hostname"] = wifiParameters.hostname;
  doc["password"] = wifiParameters.password;
  doc["data"]     = data;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();
}

void Config::print(void) {
  wifiParameters.print();
  PVL(data);
}

// Prints the content of a file to the Serial
void Config::printFile(void) {
  // Open file for reading
  File file = FILESYSTEM.open(filename, FILE_READ);
  if (!file) {
    Serial.println(F("Failed to read file"));
    return;
  }

  // Extract each characters by one by one
  while (file.available()) {
    Serial.print((char)file.read());
  }
  Serial.println();

  // Close the file
  file.close();
}