#ifndef CHIP8_EMULATOR_SUBJECT_H
#define CHIP8_EMULATOR_SUBJECT_H

#include "Observer.h"

#include <set>

template <typename T>
class Subject {
    std::set<Observer<T> *> observers_;
public:
    void add_observer(Observer<T> *o) {
        observers_.insert(o);
    }

    void remove_observer(Observer<T> *o) {
        observers_.erase(o);
    }
protected:
    void notify(const T& event) {
        for (auto &o : observers_) {
            o->on_notify(event);
        }
    }
};

#endif //CHIP8_EMULATOR_SUBJECT_H
