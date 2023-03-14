#include "framebuffer.h"

FrameBuffer::FrameBuffer(int width, int height) : _buffer_width(width), _buffer_height(height), _buffer_length(_buffer_width * _buffer_height){
    //frame_buffer = std::unique_ptr<uint32_t[]>(new uint32_t[buffer_width * buffer_height]);
    frame_buffer = std::make_unique<uint32_t[]>(buffer_width * buffer_height);

    for(auto i = 0; i < buffer_length; i++){
        frame_buffer[i] = FrameBuffer::BLACK;
    }

    return;
}

FrameBuffer::FrameBuffer() : _buffer_width(0), _buffer_height(0), _buffer_length(_buffer_width * _buffer_height){
    frame_buffer = std::make_unique<uint32_t[]>(buffer_width * buffer_height);

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

void FrameBuffer::ResizeFrameBuffer(unsigned int new_buffer_width, unsigned int new_buffer_height){
    _buffer_width = new_buffer_width;
    _buffer_height = new_buffer_height;
    _buffer_length = _buffer_width * _buffer_height;

    //frame_buffer = std::unique_ptr<uint32_t[]>(new uint32_t[buffer_width * buffer_height]);
    //frame_buffer = new uint32_t[_buffer_length];
    frame_buffer = std::make_unique<uint32_t[]>(buffer_width * buffer_height);

    for(auto i = 0; i < buffer_length; i++){
        frame_buffer[i] = FrameBuffer::BLACK;
    }
}


void FrameBuffer::ClearFrameBuffer(uint32_t color){
    
    for(auto i = 0; i < buffer_length; i++){
        frame_buffer[i] = color;
    }

    return;
}


