//
// Created by JULIA BALAJAN on 19/04/2017.
//

#ifndef CHIP8_EMULATOR_CHIP8_H
#define CHIP8_EMULATOR_CHIP8_H

#include "Drawer.h"
#include "AudioManager.h"
#include "Subject.h"
#include "Observer.h"
#include "Constants.h"

#include <SDL2/SDL_keycode.h>
#include <string>
#include <cstdint>
#include <random>
#include <unordered_map>

using namespace Constants;

class Chip8 : public Observer<int>, public Subject<uint8_t*> {
friend class Drawer;
private:
    const static uint8_t font_set[80];

    uint8_t mem_[0x1000];

    uint8_t delay_timer,
            sound_timer;

    uint8_t V_[0x10];
    uint16_t I_,
             PC_,
             stack_[12],
             SP_;
    uint16_t opcode_;

    bool key_[0x10];

    // Each bit represents a pixel
    uint8_t vga_mem_[W * H / BITS_IN_BYTE];

    // Random number generator for instruction RND
    std::mt19937 rnd_;

    // Flag which represents if the screen has been updated
    bool screen_updated;

    // Enum which represents the state of the Chip8
    CPU_state state_;

    // The register which is waiting for an input
    uint8_t* waiting_reg_;

    // Clears all values resetting to default
    void clear_();

    void clear_screen_();

    // Executes next instruction
    void exec_inst_();

    // Returns a random integer between 0, 255
    uint8_t get_rnd_num_();
public:
    Chip8(Drawer *);

    // Loads a Chip8 program into memory at the address 0x200 and onwards
    void load_program(const std::string &);

    CPU_state get_state() const;

    void emulate_cycle();
    void draw();

    virtual void on_notify(const int &key) {
        if (key == -1) {
            state_ = QUIT;
            return;
        }
        if (state_ == WAITING) {
            *waiting_reg_ = key;
            waiting_reg_ = nullptr;
            state_ = RUNNING;
        }

        key_[key] = !key_[key];
    }
};


#endif //CHIP8_EMULATOR_CHIP8_H
