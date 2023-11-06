#include <Arduino.h>
#include "utils.h"
#include "config.h"
#include "led.h"

const char *filename = "/config.txt";  // <- SD library uses 8.3 filenames
Config config;                         // <- global configuration object
Led led;  // use the builtin 

// assume we can only handle 16 tokens
int tokenize(const String& input, char *tokens[], const char* delimiter=" ") {
  static char buffer[256];
  int n=0;
  input.toCharArray(buffer,255);
  char *token = ::strtok(buffer, delimiter);
  while (token) {
    tokens[n++] = token;
    token = ::strtok(NULL,delimiter);
  }
  return n;
}

String read_serial(void) {
  String input = Serial.readString();
  input.remove(input.length()-2,2); // strip off the last 2 chars (CR, NL)
  return input;
}

void action(int n, char *tokens[]) {
  switch(*tokens[0]) {
    case 't' : 
      PVL(millis());
      break;
    case 'p' :
      print_file(filename);
      break;
    case 'v' :
      config.data = ::atoi(tokens[1]);
      break;
    case 's' :
      config_save(filename,config);
      break;
    case 'r' :
      config_load(filename,config);
      break;
    case 'c' :
      PVL(config.hostname);
      PVL(config.port);
      PVL(config.data);
      break;
    case '?' :
      PL("t  print millis()");
      PF1("p  print contents of file %s\n",filename);
      PL("v  assign value to config.data");
      PL("s  save config file");
      PL("r  load config file");
      PL("c  print config"); 
      break;
    default :
      P("unknown cmd: '"); P(tokens[0]); PL("'");
      break;
  }
  P(PROMPT);
}

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

void loop() {
  // put your main code here, to run repeatedly:
  led.blink(5);

  if (Serial.available()) {
    char *tokens[16];
    String str = read_serial();
    PL(str);
    int n_tokens = tokenize(str, tokens);
    if (n_tokens)
      action(n_tokens,tokens);
 }
}

