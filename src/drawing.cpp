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
void Drawing::DrawFilledTriangle(const Pixel &a, const Pixel &b, const Pixel &c, uint32_t color){
    
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
