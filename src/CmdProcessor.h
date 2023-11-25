#pragma once

#define MAX_CMD_BUFFER_SIZE 128
#define MAX_CMD_TOKENS  12 

class CmdProcessor {
    public:
        int     _numTokens;
        char*   _tokens[MAX_CMD_TOKENS];
        char    _buffer[MAX_CMD_BUFFER_SIZE];

        CmdProcessor() {}

        void init(void);
        void process(void);
        
    private:
        void read(void);
        void tokenize(void);
        void doCommand(void);

        void    info(void);
};

extern CmdProcessor gCmdProcessor;
