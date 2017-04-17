#ifndef CHIP8_EMULATOR_KEYLOGGER_H
#define CHIP8_EMULATOR_KEYLOGGER_H

#include <unordered_map>
#include "Subject.h"

class KeyLogger : public Subject {
    std::unordered_map<int,int> keymap{
            {SDLK_1, 0x1}, {SDLK_2, 0x2}, {SDLK_3, 0x3}, {SDLK_4, 0xC},
            {SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_r, 0xD},
            {SDLK_a, 0x7}, {SDLK_s, 0x8}, {SDLK_d, 0x9}, {SDLK_f, 0xE},
            {SDLK_z, 0xA}, {SDLK_x, 0x0}, {SDLK_c, 0xB}, {SDLK_v, 0xF},
            {SDLK_5, 0x5}, {SDLK_6, 0x6}, {SDLK_7, 0x7},
            {SDLK_8, 0x8}, {SDLK_9, 0x9}, {SDLK_0, 0x0}, {SDLK_ESCAPE,-1}
    };
public:
    void poll_events();
};


#endif //CHIP8_EMULATOR_KEYLOGGER_H
