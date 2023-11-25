#include <Arduino.h>
#include <TaskScheduler.h>
#include <RTClib.h>
#include "Utils.h"
#include "Config.h"
#include "CmdProcessor.h"
#include "Led.h"

const char *filename = "/config.txt";  // <- SD library uses 8.3 filenames
Config         gConfig;
CmdProcessor   gCmdProcessor;
Scheduler      gScheduler;
RTC_DS3231     gRTC;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void    task1Callback(void);

Task    task1(5000, TASK_FOREVER, &task1Callback);

Led     led;  // use the builtin 

void task1Callback(void) {
  P("callback: "); PL(millis());
  DateTime now = gRTC.now();
  Serial.printf("%4d/%02d/%02d (%s) %02d:%02d:%02d\n"
    ,now.year(), now.month(), now.day()
    ,daysOfTheWeek[now.dayOfTheWeek()]
    ,now.hour(), now.minute(), now.second()
  );
  P(PROMPT);
}

void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial) continue;

  led.init();
  led.off();

  if (!gRTC.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
  }

  // Should load default config if run for the first time
  gConfig.init(filename);
  // Dump config file
  PL(F("Print config file..."));
  gConfig.print();

  gScheduler.init();
  gScheduler.addTask(task1);
  task1.enable();

  gCmdProcessor.init();

  PL(); PL("starting...");
  P("compile time:  "); P(__DATE__); P(" " );  PL(__TIME__);
  P(PROMPT);
}

void loop() {
  // put your main code here, to run repeatedly:
  gScheduler.execute(); 
  led.blink(5);

  if (Serial.available()>0) 
    gCmdProcessor.process();
}

