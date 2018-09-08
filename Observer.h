#ifndef CHIP8_EMULATOR_OBSERVER_H
#define CHIP8_EMULATOR_OBSERVER_H

template <typename T>
class Observer {
public:
    virtual ~Observer() { }

    virtual void on_notify(const T& event) = 0;
};

#endif //CHIP8_EMULATOR_OBSERVER_H
