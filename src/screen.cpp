#include "screen.h"

Screen::Screen(){
    init_successful = true;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        init_successful = false;
    }

    if(SDL_GetCurrentDisplayMode(0, &display_mode) != 0 ){
        init_successful = false;
        max_width = 0;
        max_height = 0;
    }
    else{
        max_width = display_mode.w;
        max_height = display_mode.h;
    }


    window = SDL_CreateWindow(
            "3D Software Renderer",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            max_width,
            max_height,
            SDL_WINDOW_BORDERLESS);

    if(!window){
        init_successful = false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, 0x0);
    
    if(!renderer){
        init_successful = false;
    }

    if(SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0){
        init_successful = false;
    }

    texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STREAMING,
            max_width,
            max_height);

    if(!texture){
        init_successful = false;
    }
}

Screen::~Screen(){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Screen::RenderFrame(FrameBuffer &fb){
    SDL_UpdateTexture(
            texture,
            NULL,
            fb.GetFrameBuffer(),
            fb.buffer_width * sizeof(uint32_t));

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

bool Screen::InitSuccessful(){
    return init_successful;
}
