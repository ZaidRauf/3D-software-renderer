#pragma once

#include <cstdint>
#include <cstring>
#include <memory>

class FrameBuffer{
    private:
        std::unique_ptr<uint32_t[]> frame_buffer;
        unsigned int _buffer_width = 0;
        unsigned int _buffer_height = 0;
        unsigned int _buffer_length = 0;

    public:
        FrameBuffer(int width, int height);
        ~FrameBuffer();
        uint32_t* GetFrameBuffer();
        uint32_t GetPixel(int x, int y);
        void SetPixel(int x, int y, uint32_t color);
        const unsigned int &buffer_width = _buffer_width;
        const unsigned int &buffer_height = _buffer_height;
        const unsigned int &buffer_length = _buffer_length;
        void ResizeFrameBuffer(unsigned int new_buffer_width, unsigned int new_buffer_height);
        // For now assume this wont change during runtime
        static constexpr uint32_t BLACK = 0x000000FF;
};
