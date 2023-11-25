#include <Arduino.h>
#include "Utils.h"
#include "CmdProcessor.h"
#include "Config.h"

void CmdProcessor::init(void) {
  memset(_buffer,0,sizeof(_buffer));
}

void CmdProcessor::read(void) {
    init();
    String input = Serial.readString();
    input.trim();
    input.toCharArray(_buffer,MAX_CMD_BUFFER_SIZE);
}

void CmdProcessor::tokenize(void) {
  const char* delimiter = " ";
  _numTokens = 0;
  char *token = ::strtok(_buffer, delimiter);
  while (token) {
    _tokens[_numTokens++] = token;
    token = ::strtok(NULL, delimiter);
  }
}

void CmdProcessor::doCommand(void) {
  char* token = _tokens[0];
  if (!token) return;
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
    case 'i' :
      info();
      break;
    case '?' :
      PL("t  millis()");
      PL("i  compile time, info");
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
    tokenize();
    doCommand();
}

void CmdProcessor::info(void) {
  P("compile time:  "); P(__DATE__); P(" ");  PL(__TIME__);
  PVL(D0);
  PVL(D1);
  PVL(D2);
  PVL(D3);
  PVL(D4);
  PVL(D5);
  PVL(D6);
  PVL(D7);
  PVL(D8);
}