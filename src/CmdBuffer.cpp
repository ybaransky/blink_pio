#include <string.h>
#include "CmdBuffer.h"
#include "Utils.h"
#define ASCII_LF    ((char)10)
#define ASCII_CR    ((char)13)

void CmdBuffer::init() {
    flush();
}

void CmdBuffer::flush() {
    cbp = 0;
    cb[0] = (char)0;
}

// assume we can only handle 16 tokens
void CmdBuffer::tokenize(void) {
  const char* delimiter = " ";
  numTokens = 0;
  char *token = ::strtok(cb, delimiter);
  while (token) {
    tokens[numTokens++] = token;
    token = ::strtok(NULL, delimiter);
  }
}

void CmdBuffer::add(char c) {
    if (cbp > MAX_BUFFER_SIZE) 
        return;
    if ((c==ASCII_LF) || (c==ASCII_CR))
        return; // do thing
    P("adding: "); PL((int)c);
    cb[cbp++] = c;
    cb[cbp  ] = (char)0;
}

char* CmdBuffer::getToken(int i) {
    if ((i<0) || (i > numTokens)) return NULL; else return tokens[i];
}