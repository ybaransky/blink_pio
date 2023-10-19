#include <Arduino.h>

#define P(x)  Serial.print(x)
#define PL(x) Serial.println(x)
#define PV(x) P(#x); P("="); PL(x)

void print_env(void) {
  P("compile time:  "); P(__DATE__); P(" " );  PL(__TIME__);
  PV(LED_BUILTIN);
}

void setup() {
  // put your setup code here, to run once:
 //# int result = myFunction(2, 3);
  Serial.begin(9600);

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW); // turn it off

  delay(2000);
  PL();
  PL("starting...");
  print_env();
}


void loop() {
  // put your main code here, to run repeatedly:
  uint t = millis();
  P("time: "); PL(t);
  delay(1000);
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));

  print_env();
  PV(digitalRead(LED_BUILTIN));
}

