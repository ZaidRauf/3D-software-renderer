#include "drawing.h"

Drawing::Drawing(FrameBuffer &fb) : frame_buffer(fb){}

Drawing::~Drawing(){}

Drawing::Pixel::Pixel(int x, int y) : x(x), y(y){}

Drawing::Pixel::Pixel(Vector2 v) : x(v.x), y(v.y){}

Drawing::Pixel::Pixel(Vector3 v) : x(v.x), y(v.y){}

Drawing::Pixel::Pixel(Vector4 v) : x(v.x), y(v.y){}

Drawing::Pixel::~Pixel(){}

void Drawing::DrawVertex(Vector2 v, uint32_t color){
    frame_buffer.SetPixel(v.x, v.y, color);
}

// DDA Algorithm
void Drawing::DrawLine(const Pixel &start, const Pixel &end, const uint32_t color){
    // Calculate the lengths of between start and end along x and y axis
    // 
    //   /| 
    //  / | <--- delta_x
    // /  | 
    // ---- 
    //  ^ delta_x

    int dx = end.x - start.x;
    int dy = end.y - start.y;
    int long_edge_length = std::max(std::abs(dx), std::abs(dy));

    float differential_x = (float)dx / (float)long_edge_length;
    float differential_y = (float)dy / (float)long_edge_length;
    
    float current_x = start.x;
    float current_y = start.y;

    for (auto i = 0; i < long_edge_length; i++){
        frame_buffer.SetPixel(roundf(current_x), roundf(current_y), color);

        current_x += differential_x;
        current_y += differential_y;
    }

    return;
}

void Drawing::DrawTriangle(const Pixel &a, const Pixel &b, const Pixel &c, uint32_t color){
    this->DrawLine(a, b, color);
    this->DrawLine(b, c, color);
    this->DrawLine(c, a, color);
}

// Flat top flat bottom algorithm`
void Drawing::DrawFilledTriangle(const Vector4 &a, const Vector4 &b, const Vector4 &c, uint32_t color){
    Pixel v0 = Pixel(a);
    Pixel v1 = Pixel(b);
    Pixel v2 = Pixel(c);

    if(v0.y > v1.y){
        std::swap(v0, v1);
    }

    if(v1.y > v2.y){
        std::swap(v1, v2);
    }

    if(v0.y > v1.y){
        std::swap(v0, v1);
    }

    float slope_side_1 = 0;
    float slope_side_2 = 0;

    if (v1.y - v0.y != 0) slope_side_1 = static_cast<float>((v1.x - v0.x)) / static_cast<float>((v1.y - v0.y));
    if (v2.y - v0.y != 0) slope_side_2 = static_cast<float>((v2.x - v0.x)) / static_cast<float>((v2.y - v0.y));
    
    float x_start = v0.x;
    float x_end = v0.x;
    
    // If triangle is not flat top run flat bottom
    if(v1.y - v0.y != 0){
        for(auto y = v0.y; y <= v1.y; y++){
            int trunc_x_end = static_cast<int>(std::roundf(x_end));
            int trunc_x_start = static_cast<int>(std::roundf(x_start));

            if(trunc_x_end < trunc_x_start){
                std::swap(trunc_x_end, trunc_x_start);
            }

            for(int x = trunc_x_start; x <= trunc_x_end; x++){
                // Set the Z Buffer value
                // Returns weights of alpha, beta, gamma in xyz respectively
                Vector3 weights = interpolation::barycentric_weights(a, b, c, Vector2(x, y));
                float interpolated_z = 1/((weights.x/a.w) + (weights.y/b.w) + (weights.z/c.w));

                if(frame_buffer.GetZPixel(x, y) < interpolated_z){
                    frame_buffer.SetZPixel(x, y, interpolated_z);
                    frame_buffer.SetPixel(x, y, color);
                }
                //frame_buffer.SetPixel(x, y, color);
            }

            x_start += slope_side_1;
            x_end += slope_side_2;
        }
    }

    
    slope_side_1 = 0;
    if (v2.y - v1.y != 0) slope_side_1 = static_cast<float>((v2.x - v1.x)) / static_cast<float>((v2.y - v1.y));
    x_start = v2.x;
    x_end = v2.x;

    // If triangle is not flat bottom
    if(v2.y - v1.y != 0){
        for(auto y = v2.y; y >= v1.y; y--){
            int trunc_x_end = static_cast<int>(std::roundf(x_end));
            int trunc_x_start = static_cast<int>(std::roundf(x_start));

            if(trunc_x_end < trunc_x_start){
                std::swap(trunc_x_end, trunc_x_start);
            }

            for(int x = trunc_x_start; x <= trunc_x_end; x++){
               Vector3 weights = interpolation::barycentric_weights(a, b, c, Vector2(x, y));
               float interpolated_z = 1/((weights.x/a.w) + (weights.y/b.w) + (weights.z/c.w));

               if(frame_buffer.GetZPixel(x, y) < interpolated_z){
                   frame_buffer.SetZPixel(x, y, interpolated_z);
                   frame_buffer.SetPixel(x, y, color);
               }
               //frame_buffer.SetPixel(x, y, color);
            }

            x_start -= slope_side_1;
            x_end -= slope_side_2;
        }
    }
}

//void Drawing::DrawBresenhamLine(const Pixel &start, const Pixel &end, const uint32_t color){
//    int dx = end.x - start.x;
//    int dy = end.y - start.y;
//    int D = 2*dy - dx;
//    int current_y = start.y;
//
//    for (auto x = start.x; x <= end.x; x++){
//        frame_buffer.SetPixel(x, current_y, color);
//
//        if(D > 0){
//            current_y = current_y+1;
//            D -= 2 * dx;
//        }
//
//        D += 2 * dy;
//    }
//
//    return;
//}
