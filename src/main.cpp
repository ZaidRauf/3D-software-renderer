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

// TODO: Move to a settings object/singleton later
bool running = true;
bool backface_culling_enabled = false;
float translation_x = 0.0;

void exit_callback(){
    running = false;
}

void toggle_culling_callback(){
    backface_culling_enabled = !backface_culling_enabled;
}

void translation_callback(){
    translation_x += 0.1;
}

void translation2_callback(){
    translation_x -= 0.1;
}

int main(void){
    //int width = 320;
    //int height = 190;

    //FrameBuffer framebuffer = FrameBuffer(width, height);
    FrameBuffer framebuffer = FrameBuffer();
    Drawing draw = Drawing(framebuffer);
    Screen screen = Screen(framebuffer, true);
    //Screen screen = Screen(framebuffer);
    InputHandler inputhandler = InputHandler();

    int width = framebuffer.buffer_width;
    int height = framebuffer.buffer_height;

    Mesh cube = Mesh(Mesh::DefaultMesh::Cube);
    
    if(!screen.InitSuccessful() || !inputhandler.InitSuccessful()){
        std::cerr << "Failed to Initialize SDL2 Screen or InputHandler" << std::endl;
        return -1;
    }
    
    // Move to game logic type file / class later
    inputhandler.RegisterCallback(SDLK_ESCAPE, exit_callback);
    inputhandler.RegisterCallback(SDLK_c, toggle_culling_callback);
    inputhandler.RegisterCallback(SDLK_w, translation_callback);
    inputhandler.RegisterCallback(SDLK_s, translation2_callback);

    
    float rotation = 0;

    Camera camera({0,0,-5}, {0,0,1}, {0,0,0});

    while(running){
        //rotation += 0.01;
        std::vector<Triangle> rendered_triangles;

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
            world_matrix = Matrix4x4::Translation(translation_x, 0, 0) * world_matrix;

            v1 = world_matrix * v1;
            v2 = world_matrix * v2;
            v3 = world_matrix * v3;

            // Add View Matrix support
            Matrix4x4 view_matrix = Matrix4x4::ViewMatrix(camera.position, camera.target, {0, 1, 0});

            v1 = view_matrix * v1;
            v2 = view_matrix * v2;
            v3 = view_matrix * v3;

            if(backface_culling_enabled && cull::should_backface_cull(v1, v2, v3, camera.position)){
                continue;
            }

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

                if(v.x == framebuffer.buffer_width) v.x -= 1;
            }

            // retriangulate kept
            for(int i = 0; i <static_cast<int>(keep_vertex_list.size()) - 2; i++){
                Triangle t(keep_vertex_list.front(), keep_vertex_list[i+1], keep_vertex_list[i+2]);
                rendered_triangles.push_back(t);
            }
        }

        // TODO: Fragment Pass here
        for(Triangle t : rendered_triangles){
            draw.DrawTriangle(t.a, t.b, t.c, 0x00FF00FF);
            //draw.DrawVertex(t.a, 0xFF0000FF);
            //draw.DrawVertex(t.b, 0xFF0000FF);
            //draw.DrawVertex(t.c, 0xFF0000FF);
        }

        // Render what we've drawn into the framebuffer
        screen.RenderFrame(framebuffer);
        framebuffer.ClearFrameBuffer(FrameBuffer::BLACK);
        inputhandler.HandleInput();
    }

    return 0;
}
