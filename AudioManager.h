#ifndef CHIP8_EMULATOR_BEEPER_H
#define CHIP8_EMULATOR_BEEPER_H

#include <SDL2/SDL.h>
#include <deque>
#include <utility>

#include "Observer.h"

class AudioManager {
private:

    static std::deque<std::pair<uint8_t, bool>> audio_queue;
    static void callback(void* unused, uint8_t* stream, int len);

public:
    AudioManager();
    ~AudioManager();

    static AudioManager& instance();

    void beep();
};


#endif //CHIP8_EMULATOR_BEEPER_H
