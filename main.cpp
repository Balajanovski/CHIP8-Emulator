#include "Chip8.h"
#include "KeyLogger.h"

#include <string>
#include <iostream>

constexpr static int HERTZ = 60;

int main() {
    std::cout << "Please enter the CHIP8 ROM file src below:" << std::endl;

    std::string bin_file;
    while (!(std::cin >> bin_file))
        ;

    CPU::Chip8 cpu;
    KeyLogger controls;
    cpu.load_program(bin_file.c_str());
    controls.add_observer(&cpu);

    int current_time,
        last_time;
    last_time = 0;
    while (cpu.get_state() != CPU::QUIT) {
        current_time = SDL_GetTicks();
        if ((current_time - last_time) > HERTZ) {
            controls.poll_events();
            cpu.execute_instruction();
            cpu.display();
            last_time = current_time;
        }
    }

    std::cout << "Shutting down" << std::endl;

}
