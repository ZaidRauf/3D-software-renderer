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

void Drawing::DrawFilledTriangle(const Triangle &t, const Object3D &obj3d, const Light &l, const Vector3 &camera_position){
    Vector4 v0 = t.a;
    Vector4 v1 = t.b;
    Vector4 v2 = t.c;

    Vector2 uv0 = t.uv_a;
    Vector2 uv1 = t.uv_b;
    Vector2 uv2 = t.uv_c;

    Triangle::VertexInterpolants vert_int_0 = t.vert_interp_a;
    Triangle::VertexInterpolants vert_int_1 = t.vert_interp_b;
    Triangle::VertexInterpolants vert_int_2 = t.vert_interp_c;

    if(v0.y > v1.y){
        std::swap(v0, v1);
        std::swap(uv0, uv1);
        std::swap(vert_int_0, vert_int_1);
    }

    if(v1.y > v2.y){
        std::swap(v1, v2);
        std::swap(uv1, uv2);
        std::swap(vert_int_1, vert_int_2);
    }

    if(v0.y > v1.y){
        std::swap(v0, v1);
        std::swap(uv0, uv1);
        std::swap(vert_int_0, vert_int_1);
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
                float interpolated_inverse_z = ((weights.x/v0.w) + (weights.y/v1.w) + (weights.z/v2.w));
                float interpolated_z = 1/interpolated_inverse_z;

                VertexInterpolants interpolated_vertex_values = weights.x * vert_int_0 + weights.y * vert_int_1 + weights.z * vert_int_2;

                if(obj3d.perspective_correct){
                    interpolated_vertex_values.vertex_uv = ((vert_int_0.vertex_uv * (weights.x/v0.w)) + (vert_int_1.vertex_uv * (weights.y/v1.w)) + (vert_int_2.vertex_uv * (weights.z/v2.w))) * interpolated_z;
                }
                
                uint32_t color = obj3d.t.GetTexel(interpolated_vertex_values.vertex_uv.x * obj3d.t.width, interpolated_vertex_values.vertex_uv.y * obj3d.t.height);
                uint32_t color_red = (color & 0xFF000000) >> 24;
                uint32_t color_green = (color & 0x00FF0000) >> 16;
                uint32_t color_blue = (color & 0x0000FF00) >> 8;

                if(obj3d.light_type == LightingType::FLAT_LIGHTING){
                    color = (std::min((int)(t.flat_shading_intensity * color_red), 0xFF) << 24) + (std::min((int)(t.flat_shading_intensity * color_green), 0xFF) << 16) + ((int)(std::min((int)(t.flat_shading_intensity * color_blue), 0xFF)) << 8);
                    color += 0xFF;
                }
                else if(obj3d.light_type == LightingType::PHONG_SHADING){
                    float intensity = l.calculate_intensity(interpolated_vertex_values.vertex_position, interpolated_vertex_values.vertex_normal, camera_position, obj3d.tex_params);
                    color = (std::min((int)(intensity * color_red), 0xFF) << 24) + (std::min((int)(intensity * color_green), 0xFF) << 16) + ((int)(std::min((int)(intensity * color_blue), 0xFF)) << 8);
                    color += 0xFF;
                }

                // auto vec_color = vert_int_0.vertex_color * weights.x + vert_int_1.vertex_color * weights.y + vert_int_2.vertex_color * weights.z;
                // uint32_t color = (((int)vec_color.x) << 24) + (((int)vec_color.y) << 16) + (((int)vec_color.z) << 8) + 0xFF;

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
               Vector3 weights = interpolation::barycentric_weights(v0, v1, v2, Vector2(x, y));
                float interpolated_inverse_z = ((weights.x/v0.w) + (weights.y/v1.w) + (weights.z/v2.w));
                float interpolated_z = 1/interpolated_inverse_z;

                VertexInterpolants interpolated_vertex_values = weights.x * vert_int_0 + weights.y * vert_int_1 + weights.z * vert_int_2;

                if(obj3d.perspective_correct){
                    interpolated_vertex_values.vertex_uv = ((vert_int_0.vertex_uv * (weights.x/v0.w)) + (vert_int_1.vertex_uv * (weights.y/v1.w)) + (vert_int_2.vertex_uv * (weights.z/v2.w))) * interpolated_z;
                }

                uint32_t color = obj3d.t.GetTexel(interpolated_vertex_values.vertex_uv.x * obj3d.t.width, interpolated_vertex_values.vertex_uv.y * obj3d.t.height);
                uint32_t color_red = (color & 0xFF000000) >> 24;
                uint32_t color_green = (color & 0x00FF0000) >> 16;
                uint32_t color_blue = (color & 0x0000FF00) >> 8;

                if(obj3d.light_type == LightingType::FLAT_LIGHTING){
                    color = (std::min((int)(t.flat_shading_intensity * color_red), 0xFF) << 24) + (std::min((int)(t.flat_shading_intensity * color_green), 0xFF) << 16) + ((int)(std::min((int)(t.flat_shading_intensity * color_blue), 0xFF)) << 8);
                    color += 0xFF;
                }
                else if(obj3d.light_type == LightingType::PHONG_SHADING){      
                    float intensity = l.calculate_intensity(interpolated_vertex_values.vertex_position, interpolated_vertex_values.vertex_normal, camera_position, obj3d.tex_params);
                    color = (std::min((int)(intensity * color_red), 0xFF) << 24) + (std::min((int)(intensity * color_green), 0xFF) << 16) + ((int)(std::min((int)(intensity * color_blue), 0xFF)) << 8);
                    color += 0xFF;
                }

                // auto vec_color = vert_int_0.vertex_color * weights.x + vert_int_1.vertex_color * weights.y + vert_int_2.vertex_color * weights.z;
                // uint32_t color = (((int)vec_color.x) << 24) + (((int)vec_color.y) << 16) + (((int)vec_color.z) << 8) + 0xFF;

                if(frame_buffer.GetZPixel(x, y) > interpolated_z){
                    frame_buffer.SetZPixel(x, y, interpolated_z);
                    frame_buffer.SetPixel(x, y, color);
                }
            }

            x_start -= slope_side_1;
            x_end -= slope_side_2;
        }
    }
}