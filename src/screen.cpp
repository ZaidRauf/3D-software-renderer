#include "screen.h"

Screen::Screen(FrameBuffer &fb, bool setFrameBufferMaxSize, int render_scale_factor){
    init_successful = true;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        init_successful = false;
    }

    if(SDL_GetCurrentDisplayMode(0, &display_mode) != 0){
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

    unsigned int render_width = fb.buffer_width;
    unsigned int render_height = fb.buffer_height;

    //screen_pixels = std::make_unique<char*>(new char[fb.buffer_length * sizeof(uint32_t)]);
    screen_pixels = std::make_unique<RawPixelByte*>(new RawPixelByte[fb.buffer_length * sizeof(uint32_t)]);
    screen_pitch = fb.buffer_width * sizeof(uint32_t);
    screen_pixels_ptr = static_cast<void*>(screen_pixels.get());

    if(setFrameBufferMaxSize){
        render_width = max_width/render_scale_factor;
        render_height = max_height/render_scale_factor;
        fb.ResizeFrameBuffer(render_width, render_height);   
    }

    if(render_width == 0 || render_height == 0){
        init_successful = false;
    }

    texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STREAMING,
            render_width,
            render_height);

    if(!texture){
        init_successful = false;
    }
}

Screen::~Screen(){
    //delete screen_pixels;
    // TODO: CLean up screen pixels
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Screen::RenderFrame(FrameBuffer &fb){
    SDL_LockTexture(
            texture, 
            NULL, 
            &screen_pixels_ptr, 
            &screen_pitch);

    memcpy(screen_pixels_ptr, fb.GetFrameBuffer(), fb.buffer_length * sizeof(uint32_t));
    
    SDL_UnlockTexture(texture);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

bool Screen::InitSuccessful(){
    return init_successful;
}
