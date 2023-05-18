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
#include "light.h"
#include "object3d.h"

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
        void DrawFilledTriangle(const Triangle &t, const Object3D &obj3d,  const Light &l, const Vector3 &camera_position);
    private:
        FrameBuffer &frame_buffer;
};
