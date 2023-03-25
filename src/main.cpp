#include <iostream>
#include <cstdbool>
#include "screen.h"
#include "framebuffer.h"
#include "inputhandler.h"
#include "linalg.h"
#include "drawing.h"
#include "geometry.h"
#include "camera.h"
#include "clipping_culling.h"
#include "interpolation.h"
#include <vector>
#include "gamestate.h"
#include "texture.h"

float translation_x = 0.0;
float translation_y = 0.0;
float translation_z = 0.0;

GameState gamestate = GameState();

// TODO: Move game logic into it's own file
void exit_callback(){
    gamestate.running = false;
}

void toggle_culling_callback(){
    gamestate.backface_culling_enabled = !gamestate.backface_culling_enabled;
}

void translation_callback(){
    translation_x += 0.1;
}

void translation2_callback(){
    translation_x -= 0.1;
}

void translation3_callback(){
    translation_y -= 0.1;
}

void translation4_callback(){
    translation_y += 0.1;
}

void translation5_callback(){
    translation_z += 0.1;
}

void translation6_callback(){
    translation_z -= 0.1;
}

int main(){
    FrameBuffer framebuffer = FrameBuffer();
    Drawing draw = Drawing(framebuffer);
    Screen screen = Screen(framebuffer, true, 6); // Use scale parameter instead of explicit size to maintain aspect ratio
    InputHandler inputhandler = InputHandler();

    int width = framebuffer.buffer_width;
    int height = framebuffer.buffer_height;

    std::cout << width << " " << height << std::endl;

    Mesh cube = Mesh(Mesh::DefaultMesh::Cube);
    
    if(!screen.InitSuccessful() || !inputhandler.InitSuccessful()){
        std::cerr << "Failed to Initialize SDL2 Screen or InputHandler" << std::endl;
        return -1;
    }
    
    // Move to game logic type file / class later
    inputhandler.RegisterCallback(SDLK_ESCAPE, exit_callback);
    inputhandler.RegisterCallback(SDLK_c, toggle_culling_callback);
    inputhandler.RegisterCallback(SDLK_d, translation_callback);
    inputhandler.RegisterCallback(SDLK_a, translation2_callback);
    inputhandler.RegisterCallback(SDLK_w, translation3_callback);
    inputhandler.RegisterCallback(SDLK_s, translation4_callback);
    inputhandler.RegisterCallback(SDLK_q, translation5_callback);
    inputhandler.RegisterCallback(SDLK_e, translation6_callback);

    
    float rotation = 0;

    Camera camera({0,0,-5}, {0,0,1}, {0,0,0});

    while(gamestate.running){
        // Put frame time management in own function or object
        gamestate.WaitForFrame();
        float delta_time = gamestate.delta_time;
           
        rotation += 0.7 * delta_time;
        std::vector<Triangle> rendered_triangles;
        
        // TODO: Clean up lighting pass later
        std::vector<uint32_t> face_colors;

        // This loop is essentially the "Vertex Shader" of my renderer
        for (auto i = 0; i < cube.num_triangles; i++){
            Face f(cube.faces[i]);
            
            // Vertices are in (Homogenous) Model Space
            Vector4 v1 = cube.vertices[f.a - 1];
            Vector4 v2 = cube.vertices[f.b - 1];
            Vector4 v3 = cube.vertices[f.c - 1];

            // Vertices Are Transformed in World Spaace
            Matrix4x4 world_matrix = Matrix4x4::Identity();
            world_matrix = Matrix4x4::Scale(1, 1, 1);
            world_matrix = Matrix4x4::YRotationMatrix(rotation) * world_matrix;
            world_matrix = Matrix4x4::ZRotationMatrix(rotation) * world_matrix;
            world_matrix = Matrix4x4::Translation(translation_x, translation_y, translation_z) * world_matrix;

            v1 = world_matrix * v1;
            v2 = world_matrix * v2;
            v3 = world_matrix * v3;

            // Add View Matrix support
            Matrix4x4 view_matrix = Matrix4x4::ViewMatrix(camera.position, camera.target, {0, 1, 0});

            v1 = view_matrix * v1;
            v2 = view_matrix * v2;
            v3 = view_matrix * v3;

            if(gamestate.backface_culling_enabled && cull::should_backface_cull(v1, v2, v3, camera.position)){
                continue;
            }

            auto face_normal = Vector3::Cross(v1 - v2, v1 - v3).Normalized();
            Vector3 light_dir{0, 0, -1};
            
            //TODO: Move lighting calculations to own file later
            auto intensity = light_dir * face_normal;
            intensity += 0.05;
            intensity = std::max(std::min(intensity, 1.0f), 0.0f); // 0x FF FF FF FF
            //intensity += 0.2; // ambient luminence
            uint32_t color = ((int)(intensity * 0xFF) << 24) + ((int)(intensity * 0xFF) << 16) + ((int)(intensity * 0xFF) << 8);
            color += 0xFF;

            // Vertices are in Clip Space
            auto PI = 3.14159;
            float aspect_ratio = (float)framebuffer.buffer_width / (float)framebuffer.buffer_height;
            Matrix4x4 projection_matrix = Matrix4x4::PerspectiveProjectionMatrix(PI/3, aspect_ratio, 0.1, 100);

            v1 = projection_matrix * v1;
            v2 = projection_matrix * v2;
            v3 = projection_matrix * v3;

            //Proper clipping
            std::vector<Vector4> keep_vertex_list;
            clip::clip_vertices(v1, v2, v3, keep_vertex_list);

            // Vertices are in NDC
            auto persp_divide = [](Vector4 &v){
                v.x /= v.w;
                v.y /= v.w;
                v.z /= v.w;
            };
            
            // Vertices are transformed to the viewport and are ready for texturing/rasterization
            Matrix4x4 viewport_scale = Matrix4x4::Scale(framebuffer.buffer_width / 2, framebuffer.buffer_height / 2, 1.0);
            Vector4 viewport_translate(width/2, height/2, 0, 0);
            
            for(auto &v : keep_vertex_list){
                persp_divide(v);
                v = (viewport_scale * v) + viewport_translate;
                
                // Clamp kept vertices to be within buffer
                if(v.x >= framebuffer.buffer_width) v.x = framebuffer.buffer_width - 1;
                if(v.y >= framebuffer.buffer_height) v.y = framebuffer.buffer_height - 1;
                if(v.x < 0) v.x = 0;
                if(v.y < 0) v.y = 0;
            }

            // retriangulate kept
            clip::retriangulate_clipped_vertices(keep_vertex_list, rendered_triangles);

            face_colors.push_back(color);
        }

        // TODO: Fragment Pass here
       int i = 0;
       for(Triangle t : rendered_triangles){
           auto color = face_colors[i];
           i++;
           draw.DrawFilledTriangle(t.a, t.b, t.c, color);
           //draw.DrawTriangle(t.a, t.b, t.c, 0x00FF00FF);
        }

        // Render what we've drawn into the framebuffer
        screen.RenderFrame(framebuffer);
        framebuffer.ClearFrameBuffer(FrameBuffer::BLACK);
        framebuffer.ClearZBuffer();
        inputhandler.HandleInput();
    }

    return 0;
}
