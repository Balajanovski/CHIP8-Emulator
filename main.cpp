#include "Constants.h"
#include "Drawer.h"
#include "Chip8.h"
#include "Controller.h"

int main(int argc, char **argv) {
    Drawer drawer(Constants::W * Constants::PIXEL_SIZE, Constants::H * Constants::PIXEL_SIZE, "CHIP8-Emulator");
    Chip8 cpu(&drawer);
    Controller control(&cpu);
    cpu.load_program("PONG");

    int delay,
        last_ticks = 0;
    while (cpu.get_state() != Constants::QUIT) {
        control.fetch_keyboard_events();

        if (cpu.get_state() != Constants::WAITING) {
            int current_ticks = SDL_GetTicks();
            cpu.emulate_cycle();

            delay = 1000 / Constants::HERTZ - current_ticks + last_ticks;
            if (delay > 0)
                SDL_Delay(delay);

            cpu.draw();
            last_ticks = current_ticks;
        }
    }

    return 0;
}