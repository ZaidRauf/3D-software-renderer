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

    Matrix4x4 mat = Matrix4x4::Translation(1, 1, 1);
    
    std::cout << mat * Vector4(1,1,1,1) << std::endl;
    
    return 0;
};

