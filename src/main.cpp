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

const char *filename = "/config.txt";  // <- SD library uses 8.3 filenames
Config         gConfig;
CmdProcessor   gCmdProcessor;
Scheduler      gTaskScheduler;
RealTimeClock  gRealTimeClock;
Led            led;  // use the builtin 
OneButton      gButton(D6,true); 
byte           gSQWPin=D7;

byte           clk=D5;
byte           dio=D8;
TM1637Display  display(clk,dio);
void displayTest(void);
#define   TEST_DELAY  1000


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
void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial) continue;

  led.init();
  led.off();

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
}

void loop() {
  static bool firstTime= true;
  if (firstTime) {
    displayTest();
    firstTime = false;
  }

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

  gButton.tick();
}

const uint8_t SEG_DONE[] = {
	SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
	SEG_C | SEG_E | SEG_G,                           // n
	SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
	};

void displayTest(void)
{
  int k;
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
  display.setBrightness(0x0f);

  // All segments on
  PL("all segments on");
  display.setSegments(data);
  delay(TEST_DELAY);

  // Selectively set different digits
  PL("selectively set differnet digits");
  data[0] = display.encodeDigit(0);
  data[1] = display.encodeDigit(1);
  data[2] = display.encodeDigit(2);
  data[3] = display.encodeDigit(3);
  display.setSegments(data);
  delay(TEST_DELAY);

  display.clear();
  display.setSegments(data+2, 2, 2);
  delay(TEST_DELAY);

  display.clear();
  display.setSegments(data+2, 2, 1);
  delay(TEST_DELAY);

  display.clear();
  display.setSegments(data+1, 3, 1);
  delay(TEST_DELAY);


  // Show decimal numbers with/without leading zeros
  PL("Show decimal numbers with/without leading zero");
  display.showNumberDec(0, false); // Expect: ___0
  delay(TEST_DELAY);
  display.showNumberDec(0, true);  // Expect: 0000
  delay(TEST_DELAY);
	display.showNumberDec(1, false); // Expect: ___1
	delay(TEST_DELAY);
  display.showNumberDec(1, true);  // Expect: 0001
  delay(TEST_DELAY);
  display.showNumberDec(301, false); // Expect: _301
  delay(TEST_DELAY);
  display.showNumberDec(301, true); // Expect: 0301
  delay(TEST_DELAY);
  display.clear();
  display.showNumberDec(14, false, 2, 1); // Expect: _14_
  delay(TEST_DELAY);
  display.clear();
  display.showNumberDec(4, true, 2, 2);  // Expect: __04
  delay(TEST_DELAY);
  display.showNumberDec(-1, false);  // Expect: __-1
  delay(TEST_DELAY);
  display.showNumberDec(-12);        // Expect: _-12
  delay(TEST_DELAY);
  display.showNumberDec(-999);       // Expect: -999
  delay(TEST_DELAY);
  display.clear();
  display.showNumberDec(-5, false, 3, 0); // Expect: _-5_
  delay(TEST_DELAY);
  display.showNumberHexEx(0xf1af);        // Expect: f1Af
  delay(TEST_DELAY);
  display.showNumberHexEx(0x2c);          // Expect: __2C
  delay(TEST_DELAY);
  display.showNumberHexEx(0xd1, 0, true); // Expect: 00d1
  delay(TEST_DELAY);
  display.clear();
  display.showNumberHexEx(0xd1, 0, true, 2); // Expect: d1__
  delay(TEST_DELAY);
  
	// Run through all the dots
  PL("Run through all the dots");
	for(k=0; k <= 4; k++) {
		display.showNumberDecEx(0, (0x80 >> k), true);
		delay(TEST_DELAY);
	}

  // Brightness Test
  PL("Brightness Test");
  for(k = 0; k < 4; k++)
	data[k] = 0xff;
  for(k = 0; k < 7; k++) {
    display.setBrightness(k);
    display.setSegments(data);
    delay(TEST_DELAY);
  }
  
  // On/Off test
  PL("On/Off test");
  for(k = 0; k < 4; k++) {
    display.setBrightness(7, false);  // Turn off
    display.setSegments(data);
    delay(TEST_DELAY);
    display.setBrightness(7, true); // Turn on
    display.setSegments(data);
    delay(TEST_DELAY);  
  }

 
  // Done!
  PL("done");
  display.setSegments(SEG_DONE);
  PVL(millis());
}
