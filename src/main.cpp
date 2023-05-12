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
float translation_z = 0.0; // -4.5 for the bunny
float rotation = 0;

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

void rotation_callback(){
    rotation += 0.1;
}

int main(){
    FrameBuffer framebuffer = FrameBuffer(100);
    Drawing draw = Drawing(framebuffer);
    Screen screen = Screen(framebuffer, true, 3); // Use scale parameter instead of explicit size to maintain aspect ratio
    InputHandler inputhandler = InputHandler();

    int width = framebuffer.buffer_width;
    int height = framebuffer.buffer_height;

    std::cout << width << " " << height << std::endl;

    Mesh mesh = Mesh(Mesh::DefaultMesh::Cube);
    
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
    inputhandler.RegisterCallback(SDLK_t, rotation_callback);

    Camera camera({0,0,-5}, {0,0,1}, {0,0,0});

    Texture tex = Texture();

    while(gamestate.running){
        // Put frame time management in own function or object
        gamestate.WaitForFrame();
        float delta_time = gamestate.delta_time;
           
        // rotation += 0.7 * delta_time;
        std::vector<Triangle> rendered_triangles;
        
        // TODO: Clean up lighting pass later
        std::vector<uint32_t> face_colors;

        // This loop is essentially the "Vertex Shader" of my renderer
        for (auto i = 0; i < mesh.num_triangles; i++){
            Face f(mesh.faces[i]);

            // Vertices are in (Homogenous) Model Space
            Triangle t{
                mesh.vertices[f.a - 1],
                mesh.vertices[f.b - 1],
                mesh.vertices[f.c - 1],
                mesh.uv_coords[f.uv_a],
                mesh.uv_coords[f.uv_b],
                mesh.uv_coords[f.uv_c]};
            
            // Vertices Are Transformed in World Spaace
            Matrix4x4 world_matrix = Matrix4x4::Identity();
            world_matrix = Matrix4x4::Scale(1, 1, 1);
            world_matrix = Matrix4x4::YRotationMatrix(rotation) * world_matrix;
            //world_matrix = Matrix4x4::ZRotationMatrix(rotation) * world_matrix;
            world_matrix = Matrix4x4::Translation(translation_x, translation_y, translation_z) * world_matrix;
            t.MapVerts(world_matrix);

            // Add View Matrix support
            Matrix4x4 view_matrix = Matrix4x4::ViewMatrix(camera.position, camera.target, {0, 1, 0});
            t.MapVerts(view_matrix);

            if(gamestate.backface_culling_enabled && cull::should_backface_cull(t.a, t.b, t.c, camera.position)){
                continue;
            }

            // Calculate the normal for lighting calculation
            auto face_normal = Vector3::Cross(t.a - t.b, t.a - t.c).Normalized();

            //TODO: Move lighting calculations to own file later
            Vector3 light_dir{0, 0, -1};            
            auto intensity = light_dir * face_normal;
            intensity += 0.05;
            intensity = std::max(std::min(intensity, 1.0f), 0.0f);
            uint32_t color = ((int)(intensity * 0xFF) << 24) + ((int)(intensity * 0xFF) << 16) + ((int)(intensity * 0xFF) << 8);
            color += 0xFF;

            t.color = color;

            // Vertices are in Clip Space
            auto PI = 3.14159;
            float aspect_ratio = (float)framebuffer.buffer_width / (float)framebuffer.buffer_height;
            Matrix4x4 projection_matrix = Matrix4x4::PerspectiveProjectionMatrix(PI/3, aspect_ratio, 0.1, 100);
            t.MapVerts(projection_matrix);

            //Proper clipping
            std::vector<Vector4> keep_vertex_list;
            // clip::clip_vertices(t.a, t.b, t.c, keep_vertex_list);

            // std::vector<Triangle> test;
            // clip::clip_triangle(t, test);

            std::vector<Vector2> keep_uv_list;
            clip::clip_vertices_uvs(t, keep_vertex_list, keep_uv_list);

            // Vertices are in NDC
            auto persp_divide = [](Vector4 &v){
                v.x /= v.w;
                v.y /= v.w;
                v.z /= v.w;
            };
            
            // Vertices are transformed to the viewport and are ready for texturing/rasterization
            Matrix4x4 viewport_scale = Matrix4x4::Scale(framebuffer.buffer_width / 2, framebuffer.buffer_height / 2, 1.0);
            Vector4 viewport_translate(width/2, height/2, 0, 0);

            // for(auto &t : test){
            //     persp_divide(t.a);
            //     persp_divide(t.b);
            //     persp_divide(t.c);

            //     t.MapVerts(viewport_scale);

            //     // Flip as screen space grows downwards in y axis
            //     t.a.y *= -1;
            //     t.b.y *= -1;
            //     t.c.y *= -1;

            //     // Translate into centre
            //     t.a = t.a + viewport_translate;
            //     t.b = t.b + viewport_translate;
            //     t.c = t.c + viewport_translate;

            //     // Clamp kept vertices to be within buffer
            //     if(t.a.x >= framebuffer.buffer_width) t.a.x = framebuffer.buffer_width - 1;
            //     if(t.a.y >= framebuffer.buffer_height) t.a.y = framebuffer.buffer_height - 1;
            //     if(t.a.x < 0) t.a.x = 0;
            //     if(t.a.y < 0) t.a.y = 0;

            //     // Clamp kept vertices to be within buffer
            //     if(t.b.x >= framebuffer.buffer_width) t.b.x = framebuffer.buffer_width - 1;
            //     if(t.b.y >= framebuffer.buffer_height) t.b.y = framebuffer.buffer_height - 1;
            //     if(t.b.x < 0) t.a.x = 0;
            //     if(t.b.y < 0) t.a.y = 0;

            //     // Clamp kept vertices to be within buffer
            //     if(t.c.x >= framebuffer.buffer_width) t.c.x = framebuffer.buffer_width - 1;
            //     if(t.c.y >= framebuffer.buffer_height) t.c.y = framebuffer.buffer_height - 1;
            //     if(t.c.x < 0) t.a.x = 0;
            //     if(t.c.y < 0) t.a.y = 0;
            // }

            
            for(auto &v : keep_vertex_list){
                persp_divide(v);
                v = (viewport_scale * v);

                // Flip as screen space grows downwards in y axis
                v.y *= -1;

                // Translate into centre
                v = v + viewport_translate;
                
                // Clamp kept vertices to be within buffer
                if(v.x >= framebuffer.buffer_width) v.x = framebuffer.buffer_width - 1;
                if(v.y >= framebuffer.buffer_height) v.y = framebuffer.buffer_height - 1;
                if(v.x < 0) v.x = 0;
                if(v.y < 0) v.y = 0;
            }

            // retriangulate kept
            //clip::retriangulate_clipped_vertices(t, keep_vertex_list, rendered_triangles);
            clip::retriangulate_clipped_vertices_uvs(t, keep_vertex_list, keep_uv_list, rendered_triangles);

            // rendered_triangles.insert(rendered_triangles.end(), test.begin(), test.end());
        }

        // TODO: Fragment Pass here
       int i = 0;
       for(Triangle t : rendered_triangles){
           i++;
           //draw.DrawFilledTriangle(t.a, t.b, t.c, t.color);
           draw.DrawFilledTriangle(t, tex);

           //draw.DrawTriangle(t.a, t.b, t.c, 0x00FF00FF);
        }

        // Render what we've drawn into the framebuffer
        // TODO: Avoid extra memcpy by writing into screen buffer directly
        screen.RenderFrame(framebuffer);
        framebuffer.ClearFrameBuffer(FrameBuffer::BLACK);
        framebuffer.ClearZBuffer();
        inputhandler.HandleInput();
    }

    return 0;
}
