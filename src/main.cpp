#include <Arduino.h>

#define P(x)  Serial.print(x)
#define PL(x) Serial.println(x)
#define PV(x) P(#x); P("="); PL(x)

void print_environment(void) {
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
  PL(); PL("starting...");
  print_environment();
}

void blink(uint n) {
  static int count=0;
  count = count%n;
  // only blink if we are actuall off
  if (digitalRead(LED_BUILTIN))
    return;

  count++;
  PV(count);
  // blink this count times, each time more and more
  for (uint i=0; i < count; i++) {
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
    delay(100);
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
    delay(100);
  } 
}

void loop() {
  // put your main code here, to run repeatedly:
  uint t = millis();
  P("time: "); P(t); P(" "); PL(digitalRead(LED_BUILTIN));

  delay(1000);
  blink(5);
//  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
}

