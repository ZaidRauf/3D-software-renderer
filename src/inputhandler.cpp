#include "inputhandler.h"

InputHandler::InputHandler(){
    init_successful = true;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        init_successful = false;
    }

    // SDL_SetRelativeMouseMode(SDL_TRUE);
}

InputHandler::~InputHandler(){
    SDL_Quit();
}

void InputHandler::RegisterCallback(int inputKey, void (*funcPtr)()){
    key_map[inputKey] = funcPtr;
}


#include <iostream>
void InputHandler::HandleInput(){
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_KEYDOWN){
            int input_key = event.key.keysym.sym;

            if(key_map.count(input_key)){
                key_map[input_key]();
            };
        }

        // if(event.type == SDL_MOUSEMOTION){
        //     std::cout << event.motion.xrel << std::endl;
        // }
    }
}

bool InputHandler::InitSuccessful(){
    return init_successful;
}
