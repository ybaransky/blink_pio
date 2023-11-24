#pragma once

#include "CmdBuffer.h"

class CmdProcessor {
    public:
        CmdBuffer   buffer;

        CmdProcessor() {}

        void init(void);
        void process(void);
        
    private:
        void read(void);
        void action(void);
};

extern CmdProcessor gCmdProcessor;
