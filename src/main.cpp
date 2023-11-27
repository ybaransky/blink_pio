#include <Arduino.h>
#include <TaskScheduler.h>
#include <RTClib.h>
#include "Utils.h"
#include "Config.h"
#include "CmdProcessor.h"
#include "Led.h"
#include "RealTimeClock.h"

const char *filename = "/config.txt";  // <- SD library uses 8.3 filenames
Config         gConfig;
CmdProcessor   gCmdProcessor;
Scheduler      gTaskScheduler;
RealTimeClock  gRealTimeClock;
Led            led;  // use the builtin 


volatile bool EDGE;
void IRAM_ATTR sqwCallback(void) {
  EDGE = true;
}

void task1Callback(void) {
  P("callback: "); PL(millis());
  String ts  = gRealTimeClock.now().timestamp(DateTime::TIMESTAMP_FULL);
  float temp = gRealTimeClock.getTemperatureF();
  P(ts); P("  temp="); P(temp); PL("f");
  P(PROMPT);
}
Task    task1(10*1000, TASK_FOREVER, &task1Callback);

void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial) continue;

  led.init();
  led.off();

  // start the RealTimeClock
  byte pin = D6;
  EDGE = false;
  gRealTimeClock.init();
  gRealTimeClock.attachSQWInterrupt(pin,sqwCallback,FALLING);

  // Should load default config if run for the first time
  gConfig.init(filename);
  gConfig.print();

  // every 5 seconds
  gTaskScheduler.init();
  gTaskScheduler.addTask(task1);
  task1.enable();

  // parse the Serial input
  gCmdProcessor.init();

  PL(); PL("starting...");
  P("compile time:  "); P(__DATE__); P(" " );  PL(__TIME__);
  P(PROMPT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (EDGE) {
    static ulong last=0;
    ulong now = millis();
    P("EDGE ");PV(now); P(" "); PL(now - last);
    EDGE = false;
    last = now;
  }
//  gTaskScheduler.execute(); 
  led.blink(5);

  if (Serial.available()>0) 
    gCmdProcessor.process();
}

