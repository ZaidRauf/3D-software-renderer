#include "inputhandler.h"

InputHandler::InputHandler(){

}

InputHandler::~InputHandler(){

}

void InputHandler::RegisterCallback(int inputKey, void (*funcPtr)()){
    key_map[inputKey] = funcPtr;
}

void InputHandler::HandleInput(){
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_KEYDOWN){
            int input_key = event.key.keysym.sym;

            if(key_map.count(input_key)){
                key_map[input_key]();
            };
        }
    }
}
