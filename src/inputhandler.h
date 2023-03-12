#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>
#include <functional>

class InputHandler{
    public:
        InputHandler();
        ~InputHandler();
        void RegisterCallback(int inputKey, void (*funcPtr)());
        void HandleInput();

    private:
        SDL_Event event;
        std::unordered_map<int, void (*)()> key_map;
        
};
