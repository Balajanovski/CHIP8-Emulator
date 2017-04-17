#ifndef CHIP8_EMULATOR_OBSERVER_H
#define CHIP8_EMULATOR_OBSERVER_H

#include "Typedefs.h"

class Observer {
public:
    virtual ~Observer() { }
    virtual void on_notify(Bit8 key) = 0;
};

#endif //CHIP8_EMULATOR_OBSERVER_H
