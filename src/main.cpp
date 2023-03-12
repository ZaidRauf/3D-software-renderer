#include <iostream>
#include <cstdbool>
#include "screen.h"
#include "framebuffer.h"
#include "linalg.h"

int main(void){
    constexpr int width = 320;
    constexpr int height = 190;
    bool running = true;

    FrameBuffer framebuffer = FrameBuffer(width, height);
    Screen screen = Screen(framebuffer);
    
    if(!screen.InitSuccessful()){
        std::cerr << "Failed to Initialize SDL2 Screen" << std::endl;
        return -1;
    }
    
    while(running){
        framebuffer.SetPixel(100, 100, 0xFF0000FF);
        screen.RenderFrame(framebuffer);
    }

    return 0;
};

