#ifndef CHIP8_EMULATOR_SUBJECT_H
#define CHIP8_EMULATOR_SUBJECT_H

#include <set>

#include "Observer.h"
#include "Typedefs.h"

class Subject {
    std::set<Observer *> observers_;
public:
    void add_observer(Observer *o) {
        observers_.insert(o);
    }

    void remove_observer(Observer *o) {
        observers_.erase(o);
    }

    void notify(Bit8 key) {
        for (auto &o : observers_) {
            o->on_notify(key);
        }
    }
};

#endif //CHIP8_EMULATOR_SUBJECT_H
