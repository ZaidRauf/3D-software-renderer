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
    
    float x_left = v0.x;
    float x_right = v0.x;

    float slope_left = static_cast<float>((v1.x - v0.x)) / static_cast<float>((v1.y - v0.y));
    float slope_right = static_cast<float>((v2.x - v0.x)) / static_cast<float>((v2.y - v0.y));

    auto y = 0;
    for(y = v0.y; y <= v1.y; y++){
        this->DrawLine({static_cast<int>(std::roundf(x_left)), y}, {static_cast<int>(std::roundf(x_right)), y}, color);
        
        //for(auto x = x_left; x <= x_right; x++){
        //    Vector3 barycentric_weights = interpolation::barycentric_weights(v0, v1, v2, Vector2(x, y));
        //    
        //    auto alpha = barycentric_weights.x;
        //    auto beta = barycentric_weights.y;
        //    auto gamma = barycentric_weights.z;

        //    auto interpolated_z = 1/((alpha/v0.w) + (beta/v1.w) + (gamma/v2.w));

        //    //if(frame_buffer.GetZPixel(x, y) > interpolated_z){
        //        //frame_buffer.SetZPixel(x, y, interpolated_z);
        //        frame_buffer.SetPixel(x, y, color);
        //    //}
        //}

        x_left += slope_left;
        x_right += slope_right;
    }
    
    x_left = v1.x; 
    slope_left = static_cast<float>((v2.x - v1.x)) / static_cast<float>((v2.y - v1.y));
    for(; y <= v2.y; y++){
        this->DrawLine({static_cast<int>(std::roundf(x_left)), y}, {static_cast<int>(std::roundf(x_right)), y}, color);

        //for(auto x = x_left; x <= x_right; x++){
        //    Vector3 barycentric_weights = interpolation::barycentric_weights(v0, v1, v2, Vector2(x, y));
        //    
        //    auto alpha = barycentric_weights.x;
        //    auto beta = barycentric_weights.y;
        //    auto gamma = barycentric_weights.z;

        //    auto interpolated_z = 1/((alpha/v0.w) + (beta/v1.w) + (gamma/v2.w));

        //    //if(frame_buffer.GetZPixel(x, y) > interpolated_z){
        //        //frame_buffer.SetZPixel(x, y, interpolated_z);
        //        frame_buffer.SetPixel(x, y, color);
        //    //}
        //}

        x_left += slope_left;
        x_right += slope_right;
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
