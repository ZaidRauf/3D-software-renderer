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
    Vector4 v0 = a;
    Vector4 v1 = b;
    Vector4 v2 = c;

    if(v0.y > v1.y){
        std::swap(v0, v1);
    }

    if(v1.y > v2.y){
        std::swap(v1, v2);
    }

    if(v0.y > v1.y){
        std::swap(v0, v1);
    }

    Pixel p0 = Pixel(v0);
    Pixel p1 = Pixel(v1);
    Pixel p2 = Pixel(v2);

    float slope_side_1 = 0;
    float slope_side_2 = 0;

    if (p1.y - p0.y != 0) slope_side_1 = static_cast<float>((p1.x - p0.x)) / static_cast<float>((p1.y - p0.y));
    if (p2.y - p0.y != 0) slope_side_2 = static_cast<float>((p2.x - p0.x)) / static_cast<float>((p2.y - p0.y));
    
    float x_start = p0.x;
    float x_end = p0.x;
    
    // If triangle is not flat top run flat bottom
    if(p1.y - p0.y != 0){
        for(auto y = p0.y; y <= p1.y; y++){
            int trunc_x_end = static_cast<int>(std::roundf(x_end));
            int trunc_x_start = static_cast<int>(std::roundf(x_start));

            if(trunc_x_end < trunc_x_start){
                std::swap(trunc_x_end, trunc_x_start);
            }

            for(int x = trunc_x_start; x <= trunc_x_end; x++){
                // Set the Z Buffer value
                // Returns weights of alpha, beta, gamma in xyz respectively
                Vector3 weights = interpolation::barycentric_weights(v0, v1, v2, Vector2(x, y));
                float interpolated_z = 1/((weights.x/v0.w) + (weights.y/v1.w) + (weights.z/v2.w));
                
                if(frame_buffer.GetZPixel(x, y) > interpolated_z){
                    frame_buffer.SetZPixel(x, y, interpolated_z);
                    frame_buffer.SetPixel(x, y, color);
                }
                // frame_buffer.SetPixel(x, y, color);
            }

            x_start += slope_side_1;
            x_end += slope_side_2;
        }
    }
    
    slope_side_1 = 0;
    if (p2.y - p1.y != 0) slope_side_1 = static_cast<float>((p2.x - p1.x)) / static_cast<float>((p2.y - p1.y));
    x_start = p2.x;
    x_end = p2.x;

    // If triangle is not flat bottom
    if(p2.y - p1.y != 0){
        for(auto y = p2.y; y >= p1.y; y--){
            int trunc_x_end = static_cast<int>(std::roundf(x_end));
            int trunc_x_start = static_cast<int>(std::roundf(x_start));

            if(trunc_x_end < trunc_x_start){
                std::swap(trunc_x_end, trunc_x_start);
            }

            for(int x = trunc_x_start; x <= trunc_x_end; x++){
               Vector3 weights = interpolation::barycentric_weights(a, b, c, Vector2(x, y));
               float interpolated_z = 1/((weights.x/v0.w) + (weights.y/v1.w) + (weights.z/v2.w));
            //    std::cout << weights.x + weights.y + weights.z << std::endl;

               if(frame_buffer.GetZPixel(x, y) > interpolated_z){
                   frame_buffer.SetZPixel(x, y, interpolated_z);
                   frame_buffer.SetPixel(x, y, color);
               }
            //    frame_buffer.SetPixel(x, y, color);
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
