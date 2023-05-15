#pragma once

#include <memory>
#include <cstdint>
#include <fstream>
#include <iostream>

class TGAImage {
    public:
        friend class Texture;
        TGAImage(std::string fileName);
        ~TGAImage();
        enum FieldSize{
            ID_LENGTH_SIZE = 1,
            COLOR_MAP_TYPE_SIZE = 1,
            IMAGE_TYPE_SIZE = 1,
            COLOR_MAP_SPEC_SIZE = 5,
            X_ORIGIN_SIZE = 2,
            Y_ORIGIN_SIZE = 2,
            IMAGE_WIDTH_SIZE = 2,
            IMAGE_HEIGHT_SIZE = 2,
            PIXEL_DEPTH_SIZE = 1,
            IMAGE_DESCRIPTOR_SIZE = 1
        };
        enum ColorMapType{
            NO_COLOR_MAP = 0,
            COLOR_MAP = 1
        };
        enum ImageType{
            NO_IMAGE_DATA = 0,
            UNCOMPRESSED_COLOR_MAPPED = 1,
            UNCOMPRESSED_TRUE_COLOR = 2,
            UNCOMPRESSED_BLACK_AND_WHITE = 3,
            RUN_LENGTH_ENCODED_COLOR_MAPPED = 9,
            RUN_LENGTH_ENCODED_TRUE_COLOR = 10,
            RUN_LENGTH_ENCODED_BLACK_AND_WHITE = 11
        };

    private:
        uint8_t id_length;
        uint8_t color_map_type;
        uint8_t image_type;
        uint16_t x_origin;
        uint16_t y_origin;
        uint16_t width;
        uint16_t height;
        uint8_t pixel_bit_depth;
        uint8_t image_descriptor;
        unsigned short image_origin; 
        unsigned short attribute_bits_per_pixel;
        std::unique_ptr<uint32_t[]> image_data;
};