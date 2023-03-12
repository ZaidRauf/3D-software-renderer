#include <iostream>
#include <cstdbool>
#include "screen.h"
#include "framebuffer.h"
#include "inputhandler.h"
#include "linalg.h"

bool running = true;


void exit_callback(){
    running = false;
}

int main(void){
    constexpr int width = 320;
    constexpr int height = 190;

    FrameBuffer framebuffer = FrameBuffer(width, height);
    Screen screen = Screen(framebuffer);
    InputHandler inputhandler = InputHandler();
    
    if(!screen.InitSuccessful()){
        std::cerr << "Failed to Initialize SDL2 Screen" << std::endl;
        return -1;
    }

    inputhandler.RegisterCallback(SDLK_ESCAPE, exit_callback);

    while(running){
        framebuffer.SetPixel(100, 100, 0xFF0000FF);
        screen.RenderFrame(framebuffer);
        inputhandler.HandleInput();
    }

    return 0;
};

