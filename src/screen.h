#pragma once

#include <cstdbool>
#include <SDL2/SDL.h>
#include "framebuffer.h"

class Screen{
    public:
        Screen(FrameBuffer &fb);
        ~Screen();
        void RenderFrame(FrameBuffer &fb);
        bool InitSuccessful();
        
    private:
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Texture* texture = nullptr;
        SDL_DisplayMode display_mode;
        int max_width;
        int max_height;
        bool init_successful;

};
