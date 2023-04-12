#pragma once

#include <memory>
#include <cstdint>
#include <string>

class Texture {
    public:
        Texture();
        Texture(std::string &filename);
        ~Texture();
        uint32_t GetTexel(int u, int v);
        int height;
        int width;

    private:
        std::unique_ptr<uint32_t[]> texture_data;
};
