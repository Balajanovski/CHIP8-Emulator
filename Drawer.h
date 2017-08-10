#ifndef CHIP8_EMULATOR_DRAWER_H
#define CHIP8_EMULATOR_DRAWER_H

#include "Constants.h"
#include "Observer.h"

#include <SDL2/SDL.h>
#include <string>
#include <cstdint>

using namespace Constants;

class Drawer : public Observer<uint8_t *> {
    SDL_Window* win;
    SDL_Renderer* ren;

    int window_width,
        window_height;
public:
    Drawer(int, int, const std::string &);

    virtual void on_notify(uint8_t * const &vga) {

        SDL_RenderClear(ren);

        SDL_Rect dst;
        dst.x = 0;
        dst.y = 0;
        dst.w = window_width;
        dst.h = window_height;
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderFillRect(ren, &dst);
        
        uint8_t set_of_pixels;
        dst.w = PIXEL_SIZE;
        dst.h = PIXEL_SIZE;

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);


        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < (W / 8); ++x) {
                set_of_pixels = vga[y * (W / BITS_IN_BYTE) + (x)];
                for (int i = 0; i < BITS_IN_BYTE; ++i) {

                    if ((set_of_pixels >> i) & 1) {
                        dst.x = ((x * BITS_IN_BYTE) + i) * PIXEL_SIZE;
                        dst.y = y * PIXEL_SIZE;
                        SDL_RenderFillRect(ren, &dst);
                    }
                }
            }
        }

        SDL_RenderPresent(ren);

    }
};


#endif //CHIP8_EMULATOR_DRAWER_H
