#pragma once

#include <algorithm>
#include <cstdint>
#include <cmath>
#include <cstdlib>
#include "linalg.h"
#include "framebuffer.h"
#include "interpolation.h"
#include "texture.h"
#include "geometry.h"

// Executes drawing operations on the provided framebuffer
class Drawing{
    public:
        struct Pixel{
            Pixel(int x, int y);
            Pixel(Vector2 v);
            Pixel(Vector3 v);
            Pixel(Vector4 v);
            ~Pixel();
            int x;
            int y;
        };

        Drawing(FrameBuffer &fb);
        ~Drawing();
        void DrawLine(const Pixel &start, const Pixel &end, uint32_t color);
        void DrawTriangle(const Pixel &a, const Pixel &b, const Pixel &c, uint32_t color);
        void DrawVertex(Vector2 v, uint32_t color);
        void DrawFilledTriangle(const Vector4 &a, const Vector4 &b, const Vector4 &c, uint32_t color);
        void DrawFilledTriangle(const Triangle &t, const Texture &tex, bool perspectiveCorrect);
        // void DrawTexturedTriangle(const Triangle &t, const Texture &tex);
        //void DrawBresenhamLine(const Pixel &start, const Pixel &end, const uint32_t color);
    private:
        FrameBuffer &frame_buffer;
};
