#ifndef CHIP8_EMULATOR_CONTROLLER_H
#define CHIP8_EMULATOR_CONTROLLER_H

#include "Subject.h"

#include <cstdint>
#include <SDL2/SDL.h>
#include <unordered_map>

class Controller : public Subject<int> {
    // Maps regular keys to Chip8 keys
    const static std::unordered_map<SDL_Keycode, int> keymap;
public:
    Controller(Observer<int> *);
    void fetch_keyboard_events();
};


#endif //CHIP8_EMULATOR_CONTROLLER_H
