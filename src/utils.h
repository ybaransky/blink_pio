#pragma once
#include <Arduino.h>

#define P(x)   Serial.print(x)
#define PL(x)  Serial.println(x)
#define PV(x)  P(#x); P("="); P(x)
#define PVL(x) P(#x); P("="); PL(x)
#define PF1(x,y1)     Serial.printf(x,y1)
#define PF2(x,y1,y2)  Serial.printf(x,y1,y2)

#define PROMPT "-> "

#define NELEMS(x)   (sizeof(x) / sizeof((x)[0])     // number of elements in array x
