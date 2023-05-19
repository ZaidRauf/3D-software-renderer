#include "inputhandler.h"

InputHandler::InputHandler(){
    init_successful = true;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        init_successful = false;
    }

    SDL_SetRelativeMouseMode(SDL_TRUE);
}

InputHandler::~InputHandler(){
    SDL_Quit();
}

void InputHandler::RegisterCallback(int inputKey, void (*funcPtr)()){
    key_map[inputKey] = funcPtr;
}

void InputHandler::RegisterMouseMoveCallback(MouseMove mouseMoveType, void (*funcPtr)()){
    mouse_moves[mouseMoveType] = funcPtr;
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

        if(event.type == SDL_MOUSEMOTION){
            // std::cout << event.motion.xrel << std::endl;

            if(event.motion.xrel < 0){
                mouse_moves[MouseMove::MOVE_LEFT]();
            }

            if(event.motion.xrel > 0){
                mouse_moves[MouseMove::MOVE_RIGHT]();
            }

            if(event.motion.yrel < 0){
                mouse_moves[MouseMove::MOVE_DOWN]();
            }

            if(event.motion.yrel > 0){
                mouse_moves[MouseMove::MOVE_UP]();
            }
        }
    }
}

bool InputHandler::InitSuccessful(){
    return init_successful;
}
