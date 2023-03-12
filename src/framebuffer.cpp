#include "framebuffer.h"

FrameBuffer::FrameBuffer(int width, int height) : buffer_width(width), buffer_height(height), buffer_length(buffer_width * buffer_height){
    frame_buffer = std::unique_ptr<uint32_t[]>(new uint32_t[buffer_width * buffer_height]);

    for(auto i = 0; i < buffer_length; i++){
        frame_buffer[i] = FrameBuffer::BLACK;
    }

    return;
}

FrameBuffer::~FrameBuffer(){
 
}

uint32_t FrameBuffer::GetPixel(int x, int y){
    if(x < 0 || y < 0 || x >= buffer_width || y >= buffer_height){
        return 0xFFFFFFFF;
    }

    // Go to Yth row and then go to Xth column
    return frame_buffer[y * buffer_width + x];
}

void FrameBuffer::SetPixel(int x, int y, uint32_t color){
    if(x < 0 || y < 0 || x >= buffer_width || y >= buffer_height){
        return;
    }

    // Go to Yth row and then go to Xth column
    frame_buffer[y * buffer_width + x] = color;

    return;
}

uint32_t* FrameBuffer::GetFrameBuffer(){
    return frame_buffer.get();
}
