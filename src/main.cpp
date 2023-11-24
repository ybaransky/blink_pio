#include <Arduino.h>
#include "Utils.h"
#include "Config.h"
#include "CmdProcessor.h"
#include "Led.h"

const char *filename = "/config.txt";  // <- SD library uses 8.3 filenames
Config  gConfig;
CmdProcessor  gCmdProcessor;

Led     led;  // use the builtin 

void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial) continue;

  delay(2000);
  PL(); PL("starting...");
  P("compile time:  "); P(__DATE__); P(" " );  PL(__TIME__);
  // PVL(LED_BUILTIN);
  // PVL(digitalRead(LED_BUILTIN));

  led.init();
  led.off();

  gCmdProcessor.init();

  // Should load default config if run for the first time
  gConfig.init(filename);

/*
  PL(F("Loading configuration..."));
  gConfig.load();

  // Create configuration file
  PL(F("Saving configuration..."));
  gConfig.save();
*/
  // Dump config file
  PL(F("Print config file..."));
  gConfig.print();

  P(PROMPT);
}

void loop() {
  // put your main code here, to run repeatedly:
  led.blink(5);

  if (Serial.available()>0) 
    gCmdProcessor.process();
}

