#include <Arduino.h>

#define P(x)   Serial.print(x)
#define PL(x)  Serial.println(x)
#define PV(x)  P(#x); P("="); P(x)
#define PVL(x) P(#x); P("="); PL(x)

#define PROMPT "-> "

void print_environment(void) {
  P("compile time:  "); P(__DATE__); P(" " );  PL(__TIME__);
  PVL(LED_BUILTIN);
  PVL(digitalRead(LED_BUILTIN));
}

void ledOn(void) { digitalWrite(LED_BUILTIN, LOW);}
void ledOff(void) { digitalWrite(LED_BUILTIN, HIGH);}
void ledInit(void) { pinMode(LED_BUILTIN, OUTPUT);}
void ledFlip(void) { digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));}
int  ledState(void) { return digitalRead(LED_BUILTIN);}
bool ledIsOn(void) { return (LOW==digitalRead(LED_BUILTIN));}

void setup() {
  // put your setup code here, to run once:
 //# int result = myFunction(2, 3);
  Serial.begin(9600);

  ledInit();
  ledOff();

  delay(2000);
  PL(); PL("starting...");
  print_environment();
  P(PROMPT);
}

void blink(uint max_blinks) {

  static uint mini_blinks = 0; // we do this many blinks up to max_blinks
  static uint last_blink_time = 0;
  static uint max_blink_interval = 1000;
  static uint mini_blink_interval = 100;

  // only blink if the led is actually off
  if (ledIsOn())
    return;

  // only blink its long enough since last run
  if ((millis() - last_blink_time) < max_blink_interval)
    return;

  // blink mini_blink times
  mini_blinks = mini_blinks % max_blinks;
  mini_blinks++;
  for (uint i=0; i < mini_blinks; i++) {
    ledOn();  delay(mini_blink_interval);
    ledOff(); delay(mini_blink_interval);
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
    } else {
      P("unknown cmd: '"); P(str); PL("'");
    }
    P(PROMPT);
  }
}

