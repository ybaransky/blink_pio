#include <Arduino.h>
#include "Utils.h"
#include "CmdProcessor.h"
#include "Config.h"

void CmdProcessor::init(void) {
    buffer.init();
}

void CmdProcessor::read(void) {
    unsigned long tout = millis() + 5;
    P("reading ");PVL(Serial.available());
    while (Serial.available()>0) { 
        char c = Serial.read();
        P("c=");PL((int)c);
        buffer.add( c );
        if ((long)(millis() - tout) > 0) break;
    }
}

void CmdProcessor::action(void) {
  char* token = buffer.getToken(0);
  if (!token) return;

  PVL(token[0]);
  switch (token[0]) {
    case 't' : 
        PVL(millis());
        break;
    case 'g' :  // get
      gConfig.print();
      break;
    case 's' : 
      gConfig.save();
      break;
    case 'l' :
      gConfig.load();
      break;
    case '?' :
      PL("t  millis()");
      PL("g  contents of config");
      PL("s  save config file");
      PL("l  load config file");
      break;
    default :
      P("unknown cmd: '"); P(token[0]); PL("'");
    break;
  }
  P(PROMPT);
}
  
void CmdProcessor::process() {
    read();
    PVL(buffer.cb);
    buffer.tokenize();
    action();
    init();
}