#pragma once

#include <cstdint>
#include <cstring>
#include <memory>

class FrameBuffer{
    private:
        std::unique_ptr<uint32_t[]> frame_buffer;
        std::unique_ptr<float[]> z_buffer;
        unsigned int _buffer_width = 0;
        unsigned int _buffer_height = 0;
        unsigned int _buffer_length = 0;
        unsigned int _max_depth = 0;

    public:
        FrameBuffer(int max_depth);
        FrameBuffer(int width, int height, int max_depth);
        ~FrameBuffer();
        uint32_t* GetFrameBuffer();
        uint32_t* GetZBuffer();
        uint32_t GetPixel(int x, int y);
        void SetPixel(int x, int y, uint32_t color);
        float GetZPixel(int x, int y);
        void SetZPixel(int x, int y, float color);
        const unsigned int &buffer_width = _buffer_width;
        const unsigned int &buffer_height = _buffer_height;
        const unsigned int &buffer_length = _buffer_length;
        const unsigned int &max_depth = _max_depth;
        void ResizeFrameBuffer(unsigned int new_buffer_width, unsigned int new_buffer_height);
        void ClearFrameBuffer(uint32_t color);
        void ClearZBuffer();
        static constexpr uint32_t BLACK = 0x000000FF;
};
