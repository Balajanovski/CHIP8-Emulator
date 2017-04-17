#ifndef CHIP8_EMULATOR_CHIP8_H
#define CHIP8_EMULATOR_CHIP8_H

#include "Typedefs.h"
#include "Observer.h"

#include <random>
#include <SDL2/SDL.h>

namespace CPU {
    static constexpr unsigned W = 64, H = 32, PIXEL_SIZE = 16;

    enum State{RUNNING, WAITING_FOR_KEY, QUIT};

    class Chip8 : public Observer {
        Bit8 mem[0x1000] {0x12, 0x00};

        Bit8 V[16],
                I;

        Bit8    delay_timer,
                sound_timer,
                SP;

        Bit16   PC,
                stack[16];

        // Each bit represents a pixel
        Bit8 display_mem[W * H / PIXEL_SIZE];

        // System random number generator
        std::mt19937 rand{};

        // Represents the states of the keys
        Bit8 keys[16];

        // The state of the Chip8 CPU
        State state = RUNNING;

        // Pointer to the register to store the value of the next key pressed
        // Used by the LD Vx, K instruction
        Bit8 *register_waiting_key;

        virtual void on_notify(Bit8);

        // SDL objects for renderering pixels
        SDL_Window* win;
        SDL_Renderer* ren;
    public:

        void execute_instruction();
        void load_program(const char *, Bit16 pos = 0x200);
        void display();

        State get_state() const {
            return state;
        }

        Chip8();
        virtual ~Chip8();
    };
}


#endif //CHIP8_EMULATOR_CHIP8_H
