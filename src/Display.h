#pragma once
#include <TM1637Display.h>
#include <RTCLib.h>

extern void displayTest(TM1637Display&);
extern void displayCount(TM1637Display&,int count,int increment);
extern void displayNumber(TM1637Display&, int);