#include <iostream>
#include <cstdbool>
#include "screen.h"
#include "framebuffer.h"
#include "inputhandler.h"
#include "linalg.h"
#include "drawing.h"

bool running = true;

void exit_callback(){
    running = false;
}

int main(void){
    constexpr int width = 320;
    constexpr int height = 190;

    FrameBuffer framebuffer = FrameBuffer(width, height);
    Drawing draw = Drawing(framebuffer);
    Screen screen = Screen(framebuffer);
    InputHandler inputhandler = InputHandler();

    if(!screen.InitSuccessful() || !inputhandler.InitSuccessful()){
        std::cerr << "Failed to Initialize SDL2 Screen or InputHandler" << std::endl;
        return -1;
    }
    
    // Move to game logic type file / class later
    inputhandler.RegisterCallback(SDLK_ESCAPE, exit_callback);

    //framebuffer.SetPixel(width/2, height/2, 0xFF0000FF);

   //draw.DrawLine({width/2, height/2}, {10, 10}, 0x00FF00FF);
   draw.DrawLine({10, 10}, {20, height/2}, 0x0000FFFF);
    while(running){
        screen.RenderFrame(framebuffer);
        inputhandler.HandleInput();
    }

    return 0;
}
