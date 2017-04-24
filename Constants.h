#ifndef CHIP8_EMULATOR_CONSTANTS_H
#define CHIP8_EMULATOR_CONSTANTS_H

namespace Constants {
    constexpr static int W = 64,
            H = 32,
            BITS_IN_BYTE = 8,
            PIXEL_SIZE = 16,
            MS_PER_UPDATE = 8;
    enum CPU_state {RUNNING, WAITING, QUIT};
}

#endif //CHIP8_EMULATOR_CONSTANTS_H
