#include "drawing.h"

Drawing::Drawing(FrameBuffer &fb) : frame_buffer(fb){}

Drawing::~Drawing(){}

Drawing::Pixel::Pixel(int x, int y) : x(x), y(y){}

Drawing::Pixel::~Pixel(){}

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
