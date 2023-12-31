#include <Arduino.h>
#include <TaskScheduler.h>
#include <RTClib.h>
#include <OneButton.h>
#include "Utils.h"
#include "Config.h"
#include "CmdProcessor.h"
#include "RealTimeClock.h"
#include <TM1637Display.h>
#include "Display.h"

const char*     filename = "/config.txt";  // <- SD library uses 8.3 filenames
Config          gConfig;
CmdProcessor    gCmdProcessor;
//Led            led(LED_BUILTIN);  // use the builtin 

// task scheduler
void task1Callback(void) ;
Task           task1(1000, TASK_FOREVER, &task1Callback);
Scheduler      gTaskScheduler;

// push button
void           singleClick(void); 
void           doubleClick(void); 
void           longPressStart(void);
OneButton      gButton(D8,false,false);  // D8

// RTC
volatile bool EDGE;
void IRAM_ATTR sqwCallback(void);
byte           gSQWPin=D7;  // D7
RealTimeClock  gRealTimeClock;

// display
TM1637Display*  display0=nullptr;
TM1637Display*  display1=nullptr;
TM1637Display*  display2=nullptr;
bool resettedTime = false;


void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial) continue;

  // displays
  display0 = new TM1637Display(D3,D6);  // D5,D6 clk, dio
  display0->setBrightness(0x0f);
  display1 = new TM1637Display(D3,D4);  // D3,D4 clk, dio
  display1->setBrightness(0x0f);
  // how to use TX,RX pins on the wemos. 
  // https://www.youtube.com/watch?v=D9mBQ_WL7tE&t=2s
  if (true) {
    pinMode(TX,FUNCTION_3);
    // pinMode(RX,FUNCTION_3);
    display2 = new TM1637Display(D3,TX);  // RX, TX clk, dio
    display2->setBrightness(0x0f);
  }

  // start the RealTimeClock
  EDGE = false;
  resettedTime = gRealTimeClock.init();
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
    DateTime dt = gRealTimeClock.now();
    int minuteSecond = dt.minute()*100 + dt.second();
    int dayHour      = dt.day()*100 + dt.hour();
    int yearMonth    = (dt.year()*100)/100 + dt.month();
    PV(now); P(" "); P(now - last); P("  EDGE"); P(" count="); P(count);
    P(" display0: "); PV(minuteSecond); P(" display1: "); PVL(dayHour);
    displayNumber(*display0,minuteSecond);
    displayNumber(*display1,(17*millis())%10000);
    if (display2) displayNumber(*display2,count*count());
    EDGE = false;
    last = now;
    count++;
    count = count%10000;
  }
  gTaskScheduler.execute(); 
  gButton.tick();

  if (Serial.available()>0) 
    gCmdProcessor.process();
}

void IRAM_ATTR sqwCallback(void) {
  EDGE = true;
}

void singleClick(void) {
  P("Single click "); PVL(millis());
  display0->showNumberDec(1111);
  display1->showNumberDec(1111);
  if (display2) display2->showNumberDec(1111);
}

void doubleClick(void) {
  P("Double click "); PVL(millis());
  display0->showNumberDec(2222);
  display1->showNumberDec(2222);
  if (display2) display2->showNumberDec(2222);
}

void longPressStart(void) {
  // > 800ms
  P("LongPressStart "); PVL(millis());
  display0->showNumberDec(3333);
  display1->showNumberDec(3333);
  if (display2) display2->showNumberDec(3333);
}

void task1Callback(void) {
  EDGE = true;
  return;
  String ts  = gRealTimeClock.now().timestamp(DateTime::TIMESTAMP_FULL);
  float temp = gRealTimeClock.getTemperatureF();
  int now=millis();
  PV(now); P(SP); P(ts); P("  temp="); P(temp); P("f  TaskScheduler"); P(" "); PVL(resettedTime);
}