#include <iostream>
#include "screen.h"
#include "framebuffer.h"
#include "linalg.h"

int main(void){
    FrameBuffer framebuffer = FrameBuffer(320, 240);
    Screen screen = Screen();
    
    if(!screen.InitSuccessful()){
        std::cerr << "Failed to Initialize SDL2 Screen" << std::endl;
        return -1;
    }

    //while(true){
        screen.RenderFrame(framebuffer);
    //}

    return 0;
};

