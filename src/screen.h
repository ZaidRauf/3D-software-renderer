#pragma once

#include <cstdbool>
#include <SDL2/SDL.h>
#include "framebuffer.h"
#include <memory>
#include <cstring>

using RawPixelByte = char;

class Screen{
    public:
        Screen(FrameBuffer &fb, bool setFrameBufferMaxSize = false);
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
        //void* screen_pixels;
        //char* screen_pixels;
        //std::unique_ptr<char*> screen_pixels;
        std::unique_ptr<RawPixelByte*> screen_pixels;
        void* screen_pixels_ptr;
        int screen_pitch;
};
