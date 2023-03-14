#include <iostream>
#include <cstdbool>
#include "screen.h"
#include "framebuffer.h"
#include "inputhandler.h"
#include "linalg.h"
#include "drawing.h"
#include "geometry.h"

bool running = true;

void exit_callback(){
    running = false;
}

int main(void){
    //constexpr int width = 320;
    //constexpr int height = 190;

    //FrameBuffer framebuffer = FrameBuffer(width, height);
    FrameBuffer framebuffer = FrameBuffer();
    Drawing draw = Drawing(framebuffer);
    Screen screen = Screen(framebuffer, true);
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

    
    float x_translation = 0;
    float rotation = 0;
    while(running){
        x_translation += 0.01;
        rotation += 0.01;

        // This loop is essentially the "Vertex Shader" of my renderer
        for (auto i = 0; i < cube.num_triangles; i++){
            Face f(cube.faces[i]);
            
            // Vertices are in Model Space
            Vector4 v1 = cube.vertices[f.a - 1];
            Vector4 v2 = cube.vertices[f.b - 1];
            Vector4 v3 = cube.vertices[f.c - 1];
            
            // Vertices Are Transformed in World Spaace
            Matrix4x4 world_matrix = Matrix4x4::Identity();
            world_matrix = Matrix4x4::Scale(1, 1, 1);
            world_matrix = Matrix4x4::YRotationMatrix(rotation) * world_matrix;
            world_matrix = Matrix4x4::ZRotationMatrix(rotation) * world_matrix;
            world_matrix = Matrix4x4::Translation(0, 0, 5) * world_matrix;

            v1 = world_matrix * v1;
            v2 = world_matrix * v2;
            v3 = world_matrix * v3;
            
            // Vertices are in Clip Space
            auto PI = 3.14159;
            float aspect_ratio = (float)framebuffer.buffer_width / (float)framebuffer.buffer_height;
            Matrix4x4 projection_matrix = Matrix4x4::PerspectiveProjectionMatrix(PI/3, aspect_ratio, 0.1, 100);

            v1 = projection_matrix * v1;
            v2 = projection_matrix * v2;
            v3 = projection_matrix * v3;
            
            // Vertices are in NDC
            auto persp_divide = [](Vector4 &v){
                v.x /= v.w;
                v.y /= v.w;
                v.z /= v.w;
            };

            persp_divide(v1);
            persp_divide(v2);
            persp_divide(v3);

            // Vertices are transformed to the viewport and are ready for texturing/rasterization
            Matrix4x4 viewport_scale = Matrix4x4::Scale(framebuffer.buffer_width / 2, framebuffer.buffer_height / 2, 1.0);
            Vector4 viewport_translate(width/2, height/2, 0, 0);

            v1 = (viewport_scale * v1) + viewport_translate;
            v2 = (viewport_scale * v2) + viewport_translate;
            v3 = (viewport_scale * v3) + viewport_translate;

            Triangle t(
                    v1,
                    v2,
                    v3);

            draw.DrawTriangle(t.a, t.b, t.c, 0x00FF00FF);

            draw.DrawVertex(t.a, 0xFF0000FF);
            draw.DrawVertex(t.b, 0xFF0000FF);
            draw.DrawVertex(t.c, 0xFF0000FF);
        }

        // TODO: Fragment Pass here
        
        // Render what we've drawn into the framebuffer
        screen.RenderFrame(framebuffer);
        framebuffer.ClearFrameBuffer(FrameBuffer::BLACK);
        inputhandler.HandleInput();
    }

    return 0;
}
