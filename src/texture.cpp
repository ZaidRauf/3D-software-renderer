#include "texture.h"

uint32_t Texture::GetTexel(int x, int y) const{
    // Go to Yth row and then go to Xth column
    if(x < 0) x = 0;
    if(y < 0) y = 0;

    x %= width;
    y %= height;
    
    return texture_data[(y * width + x)];
}

Texture::Texture(){
    height = 4;
    width = 4;
    // Placeholder texture data
    texture_data = std::unique_ptr<uint32_t[]>( new uint32_t[]{
        0xFF0000FF, 0x00FF00FF, 0x0000FFFF, 0xFF00FFFF,
        0xFF0000FF, 0x00FF00FF, 0x0000FFFF, 0xFF00FFFF,
        0xFF0000FF, 0x00FF00FF, 0x0000FFFF, 0xFF00FFFF,
        0xFF0000FF, 0x00FF00FF, 0x0000FFFF, 0xFF00FFFF
        });

};

Texture::~Texture(){};


