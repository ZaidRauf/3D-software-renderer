#include <iostream>
#include <SDL2/SDL.h>
#include "linalg.h"

int main(void){
    std::cout << "Hello World!" << std::endl;

    Vector2 a = Vector2(1, 2);
    Vector2 b = Vector2(3, 4);
    Vector2 c = Vector2::Add(a, b);
    Vector2 d = a + b - Vector2(1,1);
    d = 3.14 * d;

    Vector2 h = d;
    h.x = 0;

    std::cout << "Vector2 (" << d.x << ", " << d.y << ") " << std::endl;
    std::cout << "Vector2 (" << h.x << ", " << h.y << ") " << std::endl;
    
    return 0;
};
