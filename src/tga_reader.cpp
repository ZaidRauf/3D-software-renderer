#include "tga_reader.h"

TGAImage::TGAImage(std::string fileName){
    std::ifstream tga_file;
    tga_file.open(fileName, std::ios::in | std::ios::binary);

    // char read_byte;
    // int read_bytes = 0;
    
    tga_file.read(reinterpret_cast<char*>(&id_length), FieldSize::ID_LENGTH_SIZE);
    tga_file.read(reinterpret_cast<char*>(&color_map_type), FieldSize::COLOR_MAP_TYPE_SIZE);
    tga_file.read(reinterpret_cast<char*>(&image_type), FieldSize::IMAGE_TYPE_SIZE);

    // Only dealing with uncompressed non color mapped RGB TGA images for now
    // std::cout << std::dec << "Position: " << tga_file.tellg() << std::endl;
    tga_file.seekg(FieldSize::COLOR_MAP_SPEC_SIZE, std::ios::cur);
    // std::cout << std::dec << "Position: " << tga_file.tellg() << std::endl;

    auto read_int_value = [&tga_file](FieldSize size){
        char int_data_buf[2];
        tga_file.read(int_data_buf, size);
        uint16_t result = (static_cast<uint16_t>(int_data_buf[1]) << 8) | static_cast<uint16_t>(int_data_buf[0]);
        return result;
    };

    x_origin = read_int_value(FieldSize::X_ORIGIN_SIZE);
    y_origin = read_int_value(FieldSize::Y_ORIGIN_SIZE);
    width = read_int_value(FieldSize::IMAGE_WIDTH_SIZE);
    height = read_int_value(FieldSize::IMAGE_HEIGHT_SIZE);
    tga_file.read(reinterpret_cast<char*>(&pixel_bit_depth), FieldSize::PIXEL_DEPTH_SIZE);
    tga_file.read(reinterpret_cast<char*>(&image_descriptor), FieldSize::IMAGE_DESCRIPTOR_SIZE);
    image_origin = image_descriptor & 0b00110000 >> 4;
    attribute_bits_per_pixel = image_descriptor & 0b1111;

    int bytes_per_pixel = pixel_bit_depth / 8;
    int image_color_data_bytes = bytes_per_pixel * width * height;

    

    image_data = std::make_unique<uint32_t[]>(width * height);

    std::cout << tga_file.tellg() << std::endl;
    char color_data_buf[4];
    for(int i = 0; i < width * height; i++){
        tga_file.read(color_data_buf, bytes_per_pixel);

        uint32_t assembled_argb_pixel_data = (static_cast<uint32_t>(color_data_buf[3]) << 24) | (static_cast<uint32_t>(color_data_buf[2]) << 16) | (static_cast<uint32_t>(color_data_buf[1]) << 8) | (static_cast<uint32_t>(color_data_buf[0]));
        assembled_argb_pixel_data &= 0x00FFFFFF;
        assembled_argb_pixel_data <<= 8;
        assembled_argb_pixel_data += 0xFF;

        image_data[i] = assembled_argb_pixel_data;
    }
    tga_file.close();
}

TGAImage::~TGAImage(){}