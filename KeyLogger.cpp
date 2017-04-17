#include <SDL2/SDL.h>

#include "KeyLogger.h"

void KeyLogger::poll_events() {
    SDL_Event ev;

    while(SDL_PollEvent(&ev)) {
        switch(ev.type) {
            case SDL_QUIT :
                notify(-1);
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                auto i = keymap.find(ev.key.keysym.sym);
                if(i == keymap.end())
                    break;
                notify(i->second);
        }
    }
}