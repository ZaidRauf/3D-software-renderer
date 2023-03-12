#pragma once

#include <cstdint>
#include <cstring>
#include <memory>

class FrameBuffer{
    public:
        FrameBuffer(int width, int height);
        ~FrameBuffer();
        uint32_t* GetFrameBuffer();
        uint32_t GetPixel(int x, int y);
        void SetPixel(int x, int y, uint32_t color);
        // For now assume this wont change during runtime
        const unsigned int buffer_width;
        const unsigned int buffer_height;
        const unsigned int buffer_length;
        static constexpr uint32_t BLACK = 0x000000FF;
    
    private:
        std::unique_ptr<uint32_t[]> frame_buffer;
};
