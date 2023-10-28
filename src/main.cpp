#include <Arduino.h>
#include "defines.h"
#include "config.h"

void print_environment(void) {
  P("compile time:  "); P(__DATE__); P(" " );  PL(__TIME__);
  PVL(LED_BUILTIN);
  PVL(digitalRead(LED_BUILTIN));
}

void led_on(void) { digitalWrite(LED_BUILTIN, LOW);}
void led_off(void) { digitalWrite(LED_BUILTIN, HIGH);}
void led_init(void) { pinMode(LED_BUILTIN, OUTPUT);}
void led_flip(void) { digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));}
int  led_state(void) { return digitalRead(LED_BUILTIN);}
bool led_ison(void) { return (LOW==digitalRead(LED_BUILTIN));}

const char *filename = "/config.txt";  // <- SD library uses 8.3 filenames
Config config;                         // <- global configuration object


void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial) continue;

  led_init();
  led_off();


  delay(2000);
  PL(); PL("starting...");
  print_environment();

  // Should load default config if run for the first time
  config_init();

  PL(F("Loading configuration..."));
  config_load(filename, config);

  // Create configuration file
  PL(F("Saving configuration..."));
  config_save(filename, config);

  // Dump config file
  PL(F("Print config file..."));
  print_file(filename);

  P(PROMPT);
}

void blink(uint max_blinks) {

  static uint mini_blinks = 0; // we do this many blinks up to max_blinks
  static uint last_blink_time = 0;
  static uint max_blink_interval = 1000;
  static uint mini_blink_interval = 50;

  // only blink if the led is actually off
  if (led_ison())
    return;

  // only blink its long enough since last run
  if ((millis() - last_blink_time) < max_blink_interval)
    return;

  // blink mini_blink times
  mini_blinks = mini_blinks % max_blinks;
  mini_blinks++;
  for (uint i=0; i < mini_blinks; i++) {
    led_on();  delay(mini_blink_interval);
    led_off(); delay(mini_blink_interval);
  } 
  last_blink_time = millis();
//  PV(millis()); P("  "); PVL(ledIsOn());
}

void loop() {
  // put your main code here, to run repeatedly:
  blink(5);
  if (Serial.available()) {
    String str = Serial.readString();
    // strip off the last 2 chars (CR, NL)
    str.remove(str.length()-2,2);
    P("recv: "); PL(str);

    if ('t' == str.charAt(0)) {
      PVL(millis());
    } else if ('l' == str.charAt(0)) {
      PVL(str.length());
      for(uint i=0;i<str.length(); i++) {
        char c = str.charAt(i);
        int  k = int(c);
        P("index: "); P(i);P(" "); PL(k);
      }
    } else if ('p' == str.charAt(0)) {
      print_file(filename);
    } else if ('v' == str.charAt(0)) {
      config.data = str.substring(2).toInt();
    } else if ('s' == str.charAt(0)) {
      config_save(filename,config);
    } else if ('r' == str.charAt(0)) {
      config_load(filename,config);
    } else if ('c' == str.charAt(0)) {
      PVL(config.hostname);
      PVL(config.port);
      PVL(config.data);
    } else {
      P("unknown cmd: '"); P(str); PL("'");
    }

    P(PROMPT);
  }
}

