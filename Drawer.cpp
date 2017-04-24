#include "Drawer.h"

#include <string>
#include <SDL2/SDL.h>

Drawer::Drawer(int w, int h, const std::string &title) : window_width(w), window_height(h) {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        std::runtime_error(std::string("error: Unable to initialise SDL ") + SDL_GetError());
    }

    win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
    if (!win) {
        std::runtime_error(std::string("error: Unable to create window ") + SDL_GetError());
    }

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (!ren) {
        std::runtime_error(std::string("error: Unable to create renderer ") + SDL_GetError());
    }
}

