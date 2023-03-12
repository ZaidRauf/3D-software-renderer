#pragma once

#include <cstdint>
#include <memory>

class FrameBuffer{
    public:
        FrameBuffer(int width, int height);
        ~FrameBuffer();
        uint32_t GetPixel(unsigned int x, unsigned int y);
        void SetPixel(unsigned int x, unsigned int y, uint32_t color);
        // For now assume this wont change during runtime
        const int buffer_width;
        const int buffer_height;
        friend float* GetBufferForDrawing();
    
    private:
        std::unique_ptr<uint32_t[]> frame_buffer;
};
