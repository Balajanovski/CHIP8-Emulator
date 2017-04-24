#include "Constants.h"
#include "Drawer.h"
#include "Chip8.h"
#include "Controller.h"

#include <iostream>

int main(int argc, char **argv) {
    // Check if the user input a rom file
    if (argc <= 1) {
        std::cout << "Please input the directory of the ROM in the arguments" << std::endl;
        return 0;
    }

    // Initialise the various components
    Drawer drawer(Constants::W * Constants::PIXEL_SIZE, Constants::H * Constants::PIXEL_SIZE, "CHIP8-Emulator");
    Chip8 cpu(&drawer);
    Controller control(&cpu);

    // Load the ROM
    cpu.load_program(argv[1]);

    // Game loop
    int delay,
        last_ticks = 0,
        lag = 0;
    while (cpu.get_state() != Constants::QUIT) {
        int current_ticks = SDL_GetTicks();
        int elapsed = current_ticks - last_ticks;
        last_ticks = current_ticks;
        lag += elapsed;

        control.fetch_keyboard_events();

        if (cpu.get_state() != Constants::WAITING) {

            while (lag >= MS_PER_UPDATE) {
                cpu.emulate_cycle();
                lag -= MS_PER_UPDATE;
            }


            cpu.draw();
        }
    }

    return 0;
}