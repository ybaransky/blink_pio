#pragma once

#include <TM1637Display.h>

class Display {
    TM1637Display*  _display;

    public:
        Display(void) ()

        void    init(byte clk, byte dio);
        

};