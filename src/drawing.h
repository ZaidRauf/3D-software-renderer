#pragma once

#include <algorithm>
#include <cstdint>
#include <cmath>
#include <cstdlib>
#include "linalg.h"
#include "framebuffer.h"

// Executes drawing operations on the provided framebuffer
class Drawing{
    public:
        struct Pixel{
            Pixel(int x, int y);
            Pixel(Vector2 V);
            ~Pixel();
            int x;
            int y;
        };

        Drawing(FrameBuffer &fb);
        ~Drawing();
        void DrawLine(const Pixel &start, const Pixel &end, uint32_t color);
    private:
        FrameBuffer &frame_buffer;
};
