//
// Created by JULIA BALAJAN on 19/04/2017.
//

#include "Chip8.h"

#include <fstream>
#include <cstdint>
#include <algorithm>
#include <random>
#include <sstream>
#include <bitset>

const uint8_t Chip8::font_set[80]  =
        {
                0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                0x20, 0x60, 0x20, 0x20, 0x70, // 1
                0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };

Chip8::Chip8(Drawer *drawer) : rnd_(std::random_device()()) {
    add_observer(drawer);
    clear_();
}

void Chip8::emulate_cycle() {
    exec_inst_();

    if (delay_timer > 0)
        --delay_timer;
    if (sound_timer > 0)
        --sound_timer;
}

void Chip8::clear_() {
    PC_            = 0x200;
    opcode_        = 0;
    I_             = 0;
    SP_            = 0;
    delay_timer    = 0;
    sound_timer    = 0;
    screen_updated = true;
    state_          = RUNNING;

    clear_screen_();
    std::fill_n(&stack_[0], 12, 0);
    std::fill_n(&V_[0], 0x10, 0);
    std::fill_n(&mem_[0], 0x1000, 0);

    // Load the font set
    for (int i = 0; i < 80; ++i)
        mem_[i] = font_set[i];
}

void Chip8::draw() {
    if (screen_updated) {
        screen_updated = false;
        notify(&vga_mem_[0]);
    }
}

void Chip8::load_program(const std::string &src) {
    int i = 0x200;
    for(std::ifstream f(src, std::ios::binary); f.good(); )
        mem_[i++ & 0xFFF] = f.get();
}

uint8_t Chip8::get_rnd_num_() {
    static std::uniform_int_distribution<uint8_t> dst(0, 255);
    return dst(rnd_);
}

inline
void Chip8::clear_screen_() {
    std::fill_n(&vga_mem_[0], W * H / BITS_IN_BYTE, 0);

}

void Chip8::exec_inst_() {
    uint16_t nnn,
              kk,
               x,
               y,
               n;

    opcode_ = mem_[PC_ & 0xFFF] << 8 | mem_[(PC_ + 1) & 0xFFF];

    switch(opcode_ & 0xF000) {
        case 0x0000:
            switch(opcode_ & 0x00FF) {
                case 0x00E0:
                    clear_screen_();
                    PC_ += 2;
                    break;

                case 0x00EE:
                    PC_ = stack_[SP_];
                    --SP_;
                    PC_ += 2;
                    break;
            }
            break;

        case 0x1000:
            nnn = opcode_ & 0x0FFF;

            PC_ = nnn;
            break;

        case 0x2000:
            nnn = opcode_ & 0x0FFF;

            stack_[++SP_] = PC_;
            PC_ = nnn;
            break;

        case 0x3000:
            x = (opcode_ & 0x0F00) >> 8;
            kk = opcode_ & 0x00FF;

            if (V_[x] == kk)
                PC_ += 2;

            PC_ += 2;
            break;

        case 0x4000:
            x = (opcode_ & 0x0F00) >> 8;
            kk = opcode_ & 0x00FF;

            if (V_[x] != kk)
                PC_ += 2;

            PC_ += 2;
            break;

        case 0x5000:
            x = (opcode_ & 0x0F00) >> 8;
            y = (opcode_ & 0x00F0) >> 4;

            if (V_[x] == V_[y])
                PC_ += 2;

            PC_ += 2;
            break;

        case 0x6000:
            x  = (opcode_ & 0x0F00) >> 8;
            kk = opcode_ & 0x00FF;

            V_[x] = kk;
            PC_ += 2;
            break;

        case 0x7000:
            x  = (opcode_ & 0x0F00) >> 8;
            kk = opcode_ & 0x00FF;

            V_[x] += kk;
            PC_ += 2;
            break;

        case 0x8000:
            switch(opcode_ & 0x000F) {
                case 0x0000:
                {
                    x = (opcode_ & 0x0F00) >> 8;
                    y = (opcode_ & 0x00F0) >> 4;

                    V_[x] = V_[y];
                    PC_ += 2;
                }
                    break;

                case 0x0001:
                {
                    x = (opcode_ & 0x0F00) >> 8;
                    y = (opcode_ & 0x00F0) >> 4;

                    V_[x] = V_[x] | V_[y];
                    PC_ += 2;
                }
                    break;

                case 0x0002:
                {
                    x = (opcode_ & 0x0F00) >> 8;
                    y = (opcode_ & 0x00F0) >> 4;

                    V_[x] = V_[x] & V_[y];
                    PC_ += 2;
                }
                    break;

                case 0x0003:
                {
                    x = (opcode_ & 0x0F00) >> 8;
                    y = (opcode_ & 0x00F0) >> 4;

                    V_[x] = V_[x] ^ V_[y];
                    PC_ += 2;
                }
                    break;

                case 0x0004:
                {
                    x = (opcode_ & 0x0F00) >> 8;
                    y = (opcode_ & 0x00F0) >> 4;

                    uint16_t sum = V_[x] + V_[y];
                    if (sum > 255)
                        V_[0xF] = 1;
                    else
                        V_[0xF] = 0;
                    V_[x] = sum & 0x00FF;
                    PC_ += 2;
                }
                    break;

                case 0x0005:
                {
                    x = (opcode_ & 0x0F00) >> 8;
                    y = (opcode_ & 0x00F0) >> 4;

                    if (V_[x] > V_[y])
                        V_[0xF] = 1;
                    else
                        V_[0xF] = 0;

                    V_[x] = V_[x] - V_[y];
                    PC_ += 2;
                }
                    break;

                case 0x0006:
                {
                    x = (opcode_ & 0x0F00) >> 8;

                    V_[0xF] = V_[x] & 0x1;

                    V_[x] >>= 1;
                    PC_ += 2;
                }
                    break;

                case 0x0007:
                {
                    x = (opcode_ & 0x0F00) >> 8;
                    y = (opcode_ & 0x00F0) >> 4;

                    if (V_[y] > V_[x])
                        V_[0xF] = 1;
                    else
                        V_[0xF] = 0;

                    V_[x] = V_[y] - V_[x];
                    PC_ += 2;
                }
                    break;

                case 0x000E:
                {
                    x = (opcode_ & 0x0F00) >> 8;

                    V_[0xF] = V_[x] >> 7;

                    V_[x] <<= 1;
                    PC_ += 2;
                }
                    break;
            }
            break;

        case 0x9000:
            x = (opcode_ & 0x0F00) >> 8;
            y = (opcode_ & 0x00F0) >> 4;

            if (V_[x] != V_[y])
                PC_ += 2;

            PC_ += 2;
            break;

        case 0xA000:
            nnn = opcode_ & 0x0FFF;
            I_ = nnn;

            PC_ += 2;
            break;

        case 0xB000:
            nnn = opcode_ & 0x0FFF;
            PC_ = nnn + V_[0x0];
            break;

        case 0xC000:
            x = (opcode_ & 0x0F00) >> 8;
            kk = opcode_ & 0x00FF;
            V_[x] = get_rnd_num_() & kk;
            PC_ += 2;
            break;

        case 0xD000:
        {
            x = (opcode_ & 0x0F00) >> 8;
            y = (opcode_ & 0x00F0) >> 4;
            n = opcode_ & 0x000F;

            V_[0xF] = 0;
            for (int y_iter = 0; y_iter < n; ++y_iter) {
                uint8_t sprite_row = mem_[I_ + y_iter];
                for (int i = 0; i < BITS_IN_BYTE; ++i) {
                    int x_iter = (i + V_[x]);
                    int bit_iter = x_iter % 8;
                    if (sprite_row & (0x80 >> i)) {
                        if ((vga_mem_[(y_iter + V_[y]) * (W / BITS_IN_BYTE) + ((x_iter) / BITS_IN_BYTE)] >> (7 - bit_iter)) & 1)
                            V_[0xF] = 1;
                        vga_mem_[(y_iter + V_[y]) * (W / BITS_IN_BYTE) + ((x_iter) / BITS_IN_BYTE)] ^= (1 << bit_iter);
                    }
                }
            }

            screen_updated = true;
            PC_ += 2;
        }
            break;

        case 0xE000:
            switch (opcode_ & 0x00FF) {
                case 0x009E:
                    x = (opcode_ & 0x0F00) >> 8;

                    if(key_[V_[x]])
                        PC_ += 2;

                    PC_+= 2;
                    break;

                case 0x00A1:
                    x = (opcode_ & 0x0F00) >> 8;

                    if(!key_[V_[x]])
                        PC_ += 2;

                    PC_+= 2;
                    break;
            }
            break;

        case 0xF000:
            switch(opcode_ & 0x00FF) {
                case 0x0007:
                {
                    x = (opcode_ & 0x0F00) >> 8;
                    V_[x] = delay_timer;

                    PC_ += 2;
                }
                    break;

                case 0x000A:
                {
                    x = (opcode_ & 0x0F00) >> 8;
                    waiting_reg_ = &V_[x];
                    state_ = WAITING;

                    PC_ += 2;
                }
                    break;

                case 0x0015:
                {
                    x = (opcode_ & 0x0F00) >> 8;
                    delay_timer = V_[x];

                    PC_ += 2;
                }
                    break;

                case 0x0018:
                {
                    x = (opcode_ & 0x0F00) >> 8;
                    sound_timer = V_[x];

                    PC_ += 2;
                }
                    break;

                case 0x001E:
                {
                    x = (opcode_ & 0x0F00) >> 8;
                    I_ += V_[x];

                    PC_ += 2;
                }
                    break;

                case 0x0029:
                {
                    x = (opcode_ & 0x0F00) >> 8;
                    I_ = V_[x] * 5;

                    PC_ += 2;
                }
                    break;

                case 0x0033:
                {
                    x = (opcode_ & 0x0F00) >> 8;
                    mem_[I_] = V_[x] / 100;
                    mem_[I_ + 1] = (V_[x] / 10) % 10;
                    mem_[I_ + 2] = (V_[x] % 100) % 10;

                    PC_ += 2;
                }
                    break;

                case 0x0055:
                {
                    x = (opcode_ & 0x0F00) >> 8;
                    for (int i = 0; i <= x; ++i)
                        mem_[I_ + i] = V_[i];

                    PC_ += 2;
                }
                    break;

                case 0x0065:
                {
                    x = (opcode_ & 0x0F00) >> 8;
                    for (int i = 0; i <= x; ++i)
                        V_[i] = mem_[I_ + i];
                    PC_ += 2;
                }
                    break;
            }
            break;
        default:
        {
            std::stringstream ss;
            ss << "Unknown opcode of value: 0x" << std::hex << opcode_;
            std::runtime_error(ss.str());
        }
            break;
    }
}

CPU_state Chip8::get_state() const {
    return state_;
}
