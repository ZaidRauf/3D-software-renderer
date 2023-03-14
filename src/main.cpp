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

    //framebuffer.SetPixel(width/2, height/2, 0xFF0000F);

   //draw.DrawLine({width/2, height/2}, {10, 10}, 0x00FF00FF);
   //draw.DrawLine({10, 10}, {20, height/2}, 0x0000FFFF);
   //draw.DrawTriangle({10, 10}, {50, 40}, {3, 100}, 0x00FF00FF);

   // for (auto i = 0; i < cube.num_triangles; i++){
   //     Face f(cube.faces[i]);
   //     
   //     auto v1 = cube.vertices[f.a - 1];
   //     auto v2 = cube.vertices[f.b - 1];
   //     auto v3 = cube.vertices[f.c - 1];

   //     v1.z += 5.0;
   //     v2.z += 5.0;
   //     v3.z += 5.0;

   //     //v1.x += 2;
   //     //v2.x += 2;
   //     //v3.x += 2;

   //     //v1.y += 2;
   //     //v2.y += 2;
   //     //v3.y += 2;

   //     v1.x /= v1.z;
   //     v1.y /= v1.z;
   //     
   //     v2.x /= v2.z;
   //     v2.y /= v2.z;
   //     
   //     v3.x /= v3.z;
   //     v3.y /= v3.z;


   //     Matrix4x4 matScale = Matrix4x4::Scale(1280, 1280, 0);
   //     v1 = matScale * v1;
   //     v2 = matScale * v2;
   //     v3 = matScale * v3;

   //     Vector3 translate(width/2, height/2, 0);
   //     v1 = v1 + translate;
   //     v2 = v2 + translate;
   //     v3 = v3 + translate;

   //     Triangle t(
   //             v1,
   //             v2,
   //             v3);

   //     //std::cout << i << " " << t.a << " " << t.b << " " << t.c << std::endl;

   //     draw.DrawTriangle(t.a, t.b, t.c, 0x00FF00FF);

   //     draw.DrawVertex(t.a, 0xFF0000FF);
   //     draw.DrawVertex(t.b, 0xFF0000FF);
   //     draw.DrawVertex(t.c, 0xFF0000FF);
   // }
    
    float x_translation = 0;
    float rotation = 0;
    while(running){
        x_translation += 0.01;
        rotation += 0.01;
        for (auto i = 0; i < cube.num_triangles; i++){
            Face f(cube.faces[i]);
            
            auto v1 = cube.vertices[f.a - 1];
            auto v2 = cube.vertices[f.b - 1];
            auto v3 = cube.vertices[f.c - 1];
            
            Matrix4x4 world_matrix = Matrix4x4::YRotationMatrix(rotation) * Matrix4x4::ZRotationMatrix(rotation);

            v1 = world_matrix * v1;
            v2 = world_matrix * v2;
            v3 = world_matrix * v3;

            v1.z += 5.0;
            v2.z += 5.0;
            v3.z += 5.0;

            //v1.x += x_translation;
            //v2.x += x_translation;
            //v3.x += x_translation;

            //v1.y += 2;
            //v2.y += 2;
            //v3.y += 2;

            v1.x /= v1.z;
            v1.y /= v1.z;
            
            v2.x /= v2.z;
            v2.y /= v2.z;
            
            v3.x /= v3.z;
            v3.y /= v3.z;


            Matrix4x4 matScale = Matrix4x4::Scale(1280, 1280, 0);
            v1 = matScale * v1;
            v2 = matScale * v2;
            v3 = matScale * v3;

            Vector3 translate(width/2, height/2, 0);
            v1 = v1 + translate;
            v2 = v2 + translate;
            v3 = v3 + translate;

            Triangle t(
                    v1,
                    v2,
                    v3);

            //std::cout << i << " " << t.a << " " << t.b << " " << t.c << std::endl;

            draw.DrawTriangle(t.a, t.b, t.c, 0x00FF00FF);

            draw.DrawVertex(t.a, 0xFF0000FF);
            draw.DrawVertex(t.b, 0xFF0000FF);
            draw.DrawVertex(t.c, 0xFF0000FF);
        }
        
        // Render what we've drawn into the framebuffer
        screen.RenderFrame(framebuffer);
        framebuffer.ClearFrameBuffer(FrameBuffer::BLACK);
        inputhandler.HandleInput();
    }

    return 0;
}
