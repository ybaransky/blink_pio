#include <Arduino.h>
#include <TaskScheduler.h>
#include <RTClib.h>
#include <OneButton.h>
#include "Utils.h"
#include "Config.h"
#include "CmdProcessor.h"
#include "Led.h"
#include "RealTimeClock.h"
#include <TM1637Display.h>
#include "Display.h"

const char*     filename = "/config.txt";  // <- SD library uses 8.3 filenames
Config          gConfig;
CmdProcessor    gCmdProcessor;
//Led            led(LED_BUILTIN);  // use the builtin 

// task scheduler
void task1Callback(void) ;
Task           task1(5*1000, TASK_FOREVER, &task1Callback);
Scheduler      gTaskScheduler;

// push button
void           singleClick(void); 
void           doubleClick(void); 
void           longPressStart(void);
OneButton      gButton(D8,false,false);  // D8

// RTC
volatile bool EDGE;
void IRAM_ATTR sqwCallback(void);
byte           gSQWPin=D7;
RealTimeClock  gRealTimeClock;

// display
TM1637Display*  display0=nullptr;
TM1637Display*  display1=nullptr;
TM1637Display*  display2=nullptr;

void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial) continue;

  // displays
  display0 = new TM1637Display(D4,D3);  // clk, dio
  display1 = new TM1637Display(D6,D5);  // clk, dio
  // how to use TX,RX pins on the wemos. 
  // https://www.youtube.com/watch?v=D9mBQ_WL7tE&t=2s
  if (true) {
    pinMode(TX,FUNCTION_3);
    pinMode(RX,FUNCTION_3);
    display2 = new TM1637Display(TX,RX);  // clk, dio
  }

  // start the RealTimeClock
  EDGE = false;
  gRealTimeClock.init();
  gRealTimeClock.attachSQWInterrupt(gSQWPin,sqwCallback,FALLING);

  // Should load default config if run for the first time
  gConfig.init(filename);
  gConfig.print();

  // every 5 seconds
  gTaskScheduler.init();
  gTaskScheduler.addTask(task1);
  task1.enable();

  // parse the Serial input
  gCmdProcessor.init();

  // button handler
  gButton.attachClick(singleClick);
  gButton.attachDoubleClick(doubleClick);
  gButton.attachLongPressStart(longPressStart);
  gButton.setClickTicks(400);

  PL(); PL("starting...");
  P("compile time:  "); P(__DATE__); P(" " );  PL(__TIME__);
  P(PROMPT);

//  displayTest(display);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (EDGE) {
    static int count = 0;
    static ulong last=0;
    ulong now = millis();
    PV(now); P(" "); P(now - last); PL("  EDGE");
    EDGE = false;
    last = now;
    count++;
    switch (count%3) {
      case 0 : displayCount(*display0, count, 1); break;
      case 1 : displayCount(*display1, count, 1); break;
      case 2 : if (display2) displayCount(*display2, count, 1); break;
    }
  }
  gTaskScheduler.execute(); 
  //led.blink(5);

  if (Serial.available()>0) 
    gCmdProcessor.process();

  gButton.tick();
}
void IRAM_ATTR sqwCallback(void) {
  EDGE = true;
}

void singleClick(void) {
  P("Single click "); PVL(millis());
}

void doubleClick(void) {
  P("Double click "); PVL(millis());
}

void longPressStart(void) {
  // > 800ms
  P("LongPressStart "); PVL(millis());
}

void task1Callback(void) {
  String ts  = gRealTimeClock.now().timestamp(DateTime::TIMESTAMP_FULL);
  float temp = gRealTimeClock.getTemperatureF();
  int now=millis();
  PV(now); P(SP); P(ts); P("  temp="); P(temp); PL("f  TaskScheduler");
}