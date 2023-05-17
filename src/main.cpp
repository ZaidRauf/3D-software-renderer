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
#include "light.h"
#include "object3d.h"
#include <list>
#include <map>

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

    // Read scene description and load models/textures into proper structures
    // Mesh m = Mesh("./assets/models/cube.obj");
    // Texture t = Texture("./assets/textures/crate.tga");

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

    Camera camera({0,0,-5}, {0, 0, 0}, {0,0,0});

    std::map<std::string, Mesh> mesh_map;
    std::map<std::string, Texture> tex_map;
    std::list<Object3D> obj_list;

    // TODO: Read scene description and load models/textures into proper structures
    mesh_map.emplace("cube", "./assets/models/cube.obj");
    tex_map.emplace("crate", "./assets/textures/crate.tga");

    Object3D obj3d = Object3D(mesh_map.at("cube"), tex_map.at("crate"));
    obj3d.position = Vector3(2, 0, 0);
    obj3d.light_type = LightingType::FULL_BRIGHT;
    obj_list.push_back(obj3d);


    mesh_map.emplace("bunny", "./assets/models/low_poly_bunny.obj");
    tex_map.emplace("gray", Texture::DefaultTexture::Gray);

    Object3D obj3d2 = Object3D(mesh_map.at("bunny"), tex_map.at("gray"));
    obj3d2.position = Vector3(-1, 0, -3);
    obj3d2.light_type = LightingType::PHONG_LIGHTING;
    obj_list.push_back(obj3d2);


    while(gamestate.running){
        // Put frame time management in own function or object
        gamestate.WaitForFrame();
        float delta_time = gamestate.delta_time;
           
        rotation += 0.7 * delta_time;
        std::vector<Triangle> rendered_triangles;

        for(auto &obj3d : obj_list){
            const Mesh &mesh = obj3d.m;
            const Texture &tex = obj3d.t;

            //TODO: Execute per frame obj updates here
            obj3d.rotation.y += 0.7 * delta_time;
            obj3d.rotation.z += 0.7 * delta_time;

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
                rotation_matrix = Matrix4x4::XRotationMatrix(obj3d.rotation.x) * rotation_matrix;
                rotation_matrix = Matrix4x4::YRotationMatrix(obj3d.rotation.y) * rotation_matrix;
                rotation_matrix = Matrix4x4::ZRotationMatrix(obj3d.rotation.z) * rotation_matrix;

                world_matrix = rotation_matrix * world_matrix;
                world_matrix = Matrix4x4::Translation(obj3d.position.x, obj3d.position.y, obj3d.position.z) * world_matrix;
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
                if(obj3d.light_type == LightingType::FLAT_LIGHTING){
                    Vector3 light_dir{0, 0, -1};            
                    auto intensity = light_dir * face_normal;
                    intensity += 0.05;
                    intensity = std::max(std::min(intensity, 1.0f), 0.0f);
                    // uint32_t color = ((int)(intensity * 0xFF) << 24) + ((int)(intensity * 0xFF) << 16) + ((int)(intensity * 0xFF) << 8);
                    // color += 0xFF;
                    t.flat_shading_intensity = intensity;
                    // t.color = color;
                }

                // Vertices are in Clip Space
                auto PI = 3.14159;
                float aspect_ratio = (float)framebuffer.buffer_width / (float)framebuffer.buffer_height;
                Matrix4x4 projection_matrix = Matrix4x4::PerspectiveProjectionMatrix(PI/3, aspect_ratio, 0.1, 100);
                t.MapVerts(projection_matrix);

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
                clip::retriangulate_clipped_vertices_uvs(t, keep_vertex_list, keep_interpolants_list, rendered_triangles);
            }

            // Fragment Pass here
            int i = 0;
            for(Triangle t : rendered_triangles){
                i++;
                //draw.DrawFilledTriangle(t.a, t.b, t.c, t.color);
                // draw.DrawFilledTriangle(t, tex, true);
                draw.DrawFilledTriangle(t, tex, true, obj3d);
                // draw.DrawTriangle(t.a, t.b, t.c, 0x00FFFFFF);
            }
        }

        // Render what we've drawn into the framebuffer
        // TODO: Avoid extra memcpy by writing into screen buffer directly
        screen.RenderFrame(framebuffer);
        framebuffer.ClearFrameBuffer(FrameBuffer::BLACK);
        framebuffer.ClearZBuffer();
        inputhandler.HandleInput();
    }

    // // Remove all references to textures and models
    while(!obj_list.empty()){
        obj_list.pop_back();
    }

    obj_list.clear();
    tex_map.clear();
    mesh_map.clear();

    return 0;
}
