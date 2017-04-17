#include "Chip8.h"

#include <SDL2/SDL.h>
#include <fstream>
#include <sstream>
#include <random>
#include <iostream>
#include <algorithm>

namespace CPU {

    static Bit8 default_rom_start[80] = {
            0xF0 ,0x90, 0x90, 0x90, 0xF0, //0
            0x20, 0x60, 0x20, 0x20, 0x70, //1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
            0x90, 0x90, 0xF0, 0x10, 0x10, //4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
            0xF0, 0x10, 0x20, 0x40, 0x40, //7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
            0xF0, 0x90, 0xF0, 0x90, 0x90, //A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
            0xF0, 0x80, 0x80, 0x80, 0xF0, //C
            0xE0, 0x90, 0x90, 0x90, 0xE0, //D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
            0xF0, 0x80, 0xF0, 0x80, 0x80  //F
    };

    void Chip8::execute_instruction() {

        if (state == WAITING_FOR_KEY) {
            return;
        }

        delay_timer = (delay_timer > 0) ? delay_timer - 1 : delay_timer;
        sound_timer = (delay_timer > 0) ? sound_timer - 1 : sound_timer;

        // Extract the two byte opcode
        Bit16 opcode = (mem[PC & 0xFFF] << 8) & mem[(PC + 1) & 0xFFF];

        unsigned nnn = (opcode >> 0) & 0xFFF;
        unsigned n   = (opcode >> 0) & 0xF;
        unsigned x   = (opcode>>8)   & 0xF;
        unsigned y   = (opcode>>4)   & 0xF;
        unsigned kk  = (opcode>>0)   & 0xFF;

        auto put_px = [this](int a, Bit8 b) {
            return ((display_mem[a] ^= b) ^ b) & b;
        };

        // CLS : Clears display_mem
        if (opcode == 0x00E0) {
            for (auto &p: display_mem)
                p = 0;
        }

            // RET : Return from subroutine
        else if (opcode == 0x00EE) {
            PC = stack[SP--];
        }

            // JP addr : Jump to location nnn
        else if ((0x1000 | nnn) == opcode) {
            PC = nnn;
        }

            // CALL addr : Call subroutine at nnn
        else if ((0x2000 | nnn) == opcode) {
            stack[SP++] = PC;
            PC = nnn;
        }

            // SE Vx, byte : Skip next instruction if Vx == kk
        else if ((0x3000 | (x << 8) | kk) == opcode) {
            if (V[x] == kk) {
                PC += 2;
            }
        }

            // SNE Vx, byte : Skip next instruction if Vx != kk
        else if ((0x4000 | (x << 8) | kk) == opcode) {
            if (V[x] != kk) {
                PC += 2;
            }
        }

            // SE Vx, Vy : Skip next instruction if Vx == Vy
        else if ((0x5000 | (x << 8) | (y << 4)) == opcode) {
            if (V[x] == V[y]) {
                PC += 2;
            }
        }

            // LD Vx, byte : Puts value kk into register Vx
        else if ((0x6000 | (x << 8) | kk) == opcode) {
            V[x] = kk;
        }

            // ADD Vx, byte : Adds kk to Vx then stores the result in Vx
        else if ((0x7000 | (x << 8) | kk) == opcode) {
            V[x] += kk;
        }

            // LD Vx, Vy : Stores the value of register Vy in register Vx
        else if ((0x8000 | (x << 8) | (y << 4)) == opcode) {
            V[x] = V[y];
        }

            // OR Vx, Vy : Stores the result of bitwise OR on Vx and Vy in Vx
        else if ((0x8000 | (x << 8) | (y << 4) | 0x0001) == opcode) {
            V[x] |= V[y];
        }

            // AND Vx, Vy : Stores the result of bitwise AND on Vx and Vy in Vx
        else if ((0x8000 | (x << 8) | (y << 4) | 0x0002) == opcode) {
            V[x] &= V[y];
        }

            // XOR Vx, Vy : Stores the result of bitwise XOR on Vx and Vy in Vx
        else if ((0x8000 | (x << 8) | (y << 4) | 0x0003) == opcode) {
            V[x] ^= V[y];
        }

            // ADD Vx, Vy : Adds Vx and Vy and stores the result in Vx
        else if ((0x8000 | (x << 8) | (y << 4) | 0x0004) == opcode) {
            Bit8 temp = V[x] + V[y];
            V[0xF] = (temp >> 8);
            V[x] = temp & 0xFF;
        }

            // SUB Vx, Vy : Subtracts Vy from Vx, then stores the result in Vx
        else if ((0x8000 | (x << 8) | (y << 4) | 0x0005) == opcode) {
            V[0xF] = static_cast<Bit8>(V[x] > V[y]);
            V[x] -= V[y];
        }

            // Shr Vx : Bitwise shifts Vx right
        else if ((0x8000 | (x << 8) | (y << 4) | 0x0006) == opcode) {
            V[0xF] = V[x] & 0x1;
            V[x] = V[x] >> 1;
        }

            // SUBN Vx, Vy : Subtracts Vx from Vy, then stores the result in Vx
        else if ((0x8000 | (x << 8) | (y << 4) | 0x0007) == opcode) {
            V[0xF] = static_cast<Bit8>(V[y] > V[x]);
            V[x] = V[y] - V[x];
        }
        else if ((0x8000 | (x << 8) | (y << 4) | 0x000E) == opcode) {
            V[0xF] = V[x] & 0x80;
            V[x] = V[x] << 1;
        }
        else if ((0x9000 | (x << 8) | (y << 4)) == opcode) {
            if (V[x] != V[y]) {
                PC += 2;
            }
        }
        else if ((0xA000 | nnn) == opcode) {
            I = nnn;
        }
        else if ((0xB000 | nnn) == opcode) {
            PC = nnn + V[0];
        }
        else if ((0xC000 | (x << 8) | kk) == opcode) {
            V[x] = std::uniform_int_distribution<>(0, 255)(rand) & kk;
        }

        // DRW : Draws n pixels of sprite from memory location I, at the coordinates x and y
        else if ((0xD000 | (x << 8) | (y << 4) | n) == opcode) {
            for (int iter = x + (W * y), i = 0, I_iter = 0;
                 i < n;
                 ++i, ++I_iter, ++iter) {
                kk |= put_px(iter, mem[(I + I_iter) & 0xFFF]) >> (    x % 8)
                      |  put_px(iter, mem[(I + I_iter) & 0xFFF]) >> (8 - x % 8);
            }
            V[0xF] = static_cast<Bit8>(kk != 0);
        }

        else if ((0xE09E | (x << 8)) == opcode) {
            if (keys[V[x] & 0xF] == 1) {
                PC += 2;
            }
        }

        else if ((0xE0A1 | (x << 8)) == opcode) {
            if (keys[V[x] & 0xF] == 0) {
                PC += 2;
            }
        }

        else if ((0xF007 | (x << 8)) == opcode) {
            V[x] = delay_timer;
        }

        else if ((0xF00A | (x << 8)) == opcode) {
            register_waiting_key = &V[x];
            state = WAITING_FOR_KEY;
        }

        else if ((0xF015 | (x << 8)) == opcode) {
            delay_timer = V[x];
        }

        else if ((0xF018 | (x << 8)) == opcode) {
            sound_timer = V[x];
        }

        else if ((0xF01E | (x << 8)) == opcode) {
            I += V[x];
        }

        else if ((0xF029 | (x << 8)) == opcode) {
            I = V[x] * 0x5;
        }

        else if ((0xF033 | (x << 8)) == opcode) {
            Bit8 temp = V[x] % 100;

            mem[I] = V[x] / 100;
            mem[I + 1] = (V[x] / 10) % 10;
            mem[I + 2] = temp % 10;
        }

        else if ((0xF055 | (x << 8))) {
            for (size_t i = 0; i <= x; ++i) {
                mem[I + i] = V[i];
            }
        }

        else if ((0xF065 | (x << 8))) {
            for (size_t i = 0; i <= x; ++i) {
                V[i] = mem[I + i];
            }
        }

        else {
            std::stringstream ss;
            ss << "error: Unknown opcode with the value of: " << std::hex << opcode;
            throw std::runtime_error(ss.str());
        }

        PC += 2;
    }

    void Chip8::load_program(const char *src, Bit16 pos) {
        for (std::ifstream f(src, std::ios::binary | std::ios::in); f.good(); ) {
            mem[pos++ & 0xFFF] = f.get();
        }
    }

    void Chip8::on_notify(Bit8 key) {
        if (state == WAITING_FOR_KEY) {
            *register_waiting_key = key;
            state = RUNNING;
        }

        // Quit event
        if (key == -1) {
            state = QUIT;
        }

        keys[key] = !keys[key];

    }

    Chip8::Chip8() {
        if(SDL_Init(SDL_INIT_EVERYTHING)) {
            throw std::runtime_error("Unable to initialize SDL");
        }

        win = SDL_CreateWindow("Tron", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W * PIXEL_SIZE, H * PIXEL_SIZE, SDL_WINDOW_SHOWN);
        if (!win)
            throw std::runtime_error("Window Initialisation");

        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
        if (!ren)
            throw std::runtime_error("Renderer Initialisation");

        std::fill(std::begin(V)      , std::end(V)      ,0);
        std::fill(std::begin(mem)    , std::end(mem)    ,0);
        std::fill(std::begin(stack)  , std::end(stack)  ,0);
        std::fill(std::begin(keys)   , std::end(keys)   ,0);
        std::fill(std::begin(display_mem), std::end(display_mem),0);

        SP = PC = delay_timer = sound_timer = 0;

        for (size_t i = 0; i < 80; ++i) {
            mem[i] = default_rom_start[i];
        }
    }

    Chip8::~Chip8() {
        SDL_DestroyWindow(win);
        SDL_DestroyRenderer(ren);

        SDL_Quit();
    }

    void Chip8::display() {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 1);
        const static SDL_Rect SCREEN = {W * PIXEL_SIZE, H * PIXEL_SIZE, 0, 0};
        SDL_RenderFillRect(ren, &SCREEN);

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_Rect dst;
        dst.w = PIXEL_SIZE;
        dst.h = PIXEL_SIZE;

        int i = 0;
        for (; i < (W * H / 8); ++i) {
            if (display_mem[i]) {
                dst.x = i / CPU::W;
                dst.y = i % CPU::W;
                SDL_RenderFillRect(ren, &dst);
            }
        }
        SDL_RenderPresent(ren);
    }
}


