#pragma once

#define MAX_BUFFER_SIZE 128
#define MAX_NUM_TOKENS  12 

class CmdBuffer {
    public:
        int     numTokens;
        char*   tokens[MAX_NUM_TOKENS];
        
        int     cbp;
        char    cb[MAX_BUFFER_SIZE];

        CmdBuffer() {}

        void    init(void);
        void    flush(void);
        void    add(char);
        void    tokenize(void);
        char*   getToken(int);
};