#include "AudioManager.h"

#include <string>
#include <cmath>
#include <iostream>

std::deque<std::pair<uint8_t, bool>> AudioManager::audio_queue;

void AudioManager::callback(void *unused, uint8_t *stream, int len) {

    // If the audio queue is empty fill up the stream with 0s
    if (audio_queue.empty()) {
        for (int i = 0; i < len; ++i) {
            stream[i] = 0;
        }
    }
    else {

        // Generate square wave
        short* target = (short*)stream;
        while(len > 0 && !audio_queue.empty())
        {
            auto& data = audio_queue.front();

            for(; len && data.first; target += 2, len -= 4, --data.first) {
                target[0] = target[1] = data.second * 300 * ((len&128)-64);
            }

            if(!data.first) {
                audio_queue.pop_front();
            }
        }
    }
}

AudioManager::AudioManager() {
    SDL_AudioSpec spec;

    // Initialise the SDL audio.
    spec.freq = 11000;
    spec.format = AUDIO_S16SYS;
    spec.channels = 2;
    spec.samples = 11000 / 20;
    spec.callback = callback;

    // Error handling
    int error_code = SDL_OpenAudio(&spec, NULL);
    if (error_code != 0) {
        std::string error = "error: SDL_audio was unable to be initialised with an error code of" + std::to_string(error_code) + "\nError message:" + std::string(SDL_GetError());
        throw std::runtime_error(error);
    }

    SDL_PauseAudio(0);
}

AudioManager::~AudioManager() {
    SDL_CloseAudio();
}

AudioManager& AudioManager::instance() {

    static AudioManager *instance_ = new AudioManager();
    return *instance_;
}

void AudioManager::beep() {
    // Attach wave to audio queue. True represents higher part of square wave while false represents lower part of square wave
    audio_queue.emplace_back(500000, true);
    audio_queue.emplace_back(500000, false);
}