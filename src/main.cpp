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
#include "tga_reader.h"
#include "obj_loader.h"

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
    // auto o = OBJLoader("./assets/models/low_poly_bunny.obj");
    FrameBuffer framebuffer = FrameBuffer(100);
    Drawing draw = Drawing(framebuffer);
    Screen screen = Screen(framebuffer, true, 6); // Use scale parameter instead of explicit size to maintain aspect ratio
    InputHandler inputhandler = InputHandler();

    int width = framebuffer.buffer_width;
    int height = framebuffer.buffer_height;

    std::cout << width << " " << height << std::endl;

    // Mesh mesh = Mesh(Mesh::DefaultMesh::Cube);
    // Texture tex = Texture(Texture::DefaultTexture::Gray);
    Mesh mesh = Mesh("./assets/models/cube.obj");
    Texture tex = Texture("./assets/textures/crate.tga");

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

    while(gamestate.running){
        // Put frame time management in own function or object
        gamestate.WaitForFrame();
        float delta_time = gamestate.delta_time;
           
        rotation += 0.7 * delta_time;
        std::vector<Triangle> rendered_triangles;
        
        // This loop is essentially the "Vertex Shader" of my renderer
        for (auto i = 0; i < mesh.num_triangles; i++){
            const Face &f = mesh.faces[i];

            // Vertices are in (Homogenous) Model Space
            Triangle t{f, mesh};
                
            // Vertices Are Transformed in World Spaace
            Matrix4x4 world_matrix = Matrix4x4::Identity();
            world_matrix = Matrix4x4::Scale(1, 1, 1);

            // Seperate from world matrix as we use to rotate face and vertex normals
            Matrix4x4 rotation_matrix = Matrix4x4::Scale(1, 1, 1);
            //rotation_matrix = Matrix4x4::XRotationMatrix(rotation) * rotation_matrix;
            rotation_matrix = Matrix4x4::YRotationMatrix(rotation) * rotation_matrix;
            rotation_matrix = Matrix4x4::ZRotationMatrix(rotation) * rotation_matrix;

            world_matrix = rotation_matrix * world_matrix;
            world_matrix = Matrix4x4::Translation(translation_x, translation_y, translation_z) * world_matrix;
            t.MapVerts(world_matrix);

            // Add View Matrix support
            Matrix4x4 view_matrix = Matrix4x4::ViewMatrix(camera.position, camera.target, {0, 1, 0});
            t.MapVerts(view_matrix);

            if(gamestate.backface_culling_enabled && cull::should_backface_cull(t.a, t.b, t.c, camera.position)){
                continue;
            }

            t.TransformInterpolants(rotation_matrix, world_matrix);

            // Calculate the normal for lighting calculation
            Vector3 face_normal = rotation_matrix * mesh.face_normals[i];

            //Flat Lighting Pass
            Vector3 light_dir{0, 0, -1};            
            auto intensity = light_dir * face_normal;
            intensity += 0.05;
            intensity = std::max(std::min(intensity, 1.0f), 0.0f);
            uint32_t color = ((int)(intensity * 0xFF) << 24) + ((int)(intensity * 0xFF) << 16) + ((int)(intensity * 0xFF) << 8);
            color += 0xFF;

            t.flat_shading_intensity = intensity;
            t.color = color;

            // Vertices are in Clip Space
            auto PI = 3.14159;
            float aspect_ratio = (float)framebuffer.buffer_width / (float)framebuffer.buffer_height;
            Matrix4x4 projection_matrix = Matrix4x4::PerspectiveProjectionMatrix(PI/3, aspect_ratio, 0.1, 100);
            t.MapVerts(projection_matrix);

            //Proper clipping
            // std::vector<Vector4> keep_vertex_list;
            // std::vector<Vector2> keep_uv_list;
            // clip::clip_vertices_uvs(t, keep_vertex_list, keep_uv_list);

            std::vector<Vector4> keep_vertex_list;
            std::vector<Triangle::VertexInterpolants> keep_interpolants_list;
            clip::clip_vertices_uvs(t, keep_vertex_list, keep_interpolants_list);

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
            // clip::retriangulate_clipped_vertices_uvs(t, keep_vertex_list, keep_uv_list, rendered_triangles);
            clip::retriangulate_clipped_vertices_uvs(t, keep_vertex_list, keep_interpolants_list, rendered_triangles);
        }

        // TODO: Fragment Pass here
       int i = 0;
       for(Triangle t : rendered_triangles){
           i++;
           //draw.DrawFilledTriangle(t.a, t.b, t.c, t.color);
           draw.DrawFilledTriangle(t, tex, true);
           draw.DrawTriangle(t.a, t.b, t.c, 0x00FFFFFF);
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
