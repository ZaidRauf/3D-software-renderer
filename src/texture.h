#pragma once

#include <memory>
#include <cstdint>
#include <string>
#include <algorithm>
#include "tga_reader.h"

class Texture {
    public:
        enum DefaultTexture {
            Gray,
            Test,
            Test2
        };
        Texture();
        Texture(const TGAImage &tga_img);
        Texture(std::string &filename);
        Texture(enum DefaultTexture default_texture_idx);
        ~Texture();
        uint32_t GetTexel(int u, int v) const;
        int height;
        int width;

    private:
        std::unique_ptr<uint32_t[]> texture_data;
};
