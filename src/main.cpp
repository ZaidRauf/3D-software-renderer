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
#include "scene.h"

GameState gamestate = GameState();
Camera camera({0,0,-5}, {0, 0, 0}, {0,1,0});
// float rot = 0.0;

// TODO: Move game logic into it's own file
void exit_callback(){
    gamestate.running = false;
}

void toggle_culling_callback(){
    gamestate.backface_culling_enabled = !gamestate.backface_culling_enabled;
}

void camera_forward() {
    camera.camera_forward();
}

void camera_backward() {
    camera.camera_backward();
}

void camera_strafe_left() {
    camera.camera_strafe_left();
}

void camera_strafe_right() {
    camera.camera_strafe_right();
}

void camera_rotate_left() {
    camera.camera_rotate_left();
}

void camera_rotate_right() {
    camera.camera_rotate_right();
}

void camera_rotate_up() {
    camera.camera_rotate_up();
}

void camera_rotate_down() {
    camera.camera_rotate_down();
}

void camera_rotate_left_big() {
    camera.camera_rotate_left_big();
}

void camera_rotate_right_big() {
    camera.camera_rotate_right_big();
}

void camera_rotate_up_big() {
    camera.camera_rotate_up_big();
}

void camera_rotate_down_big() {
    camera.camera_rotate_down_big();
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

    if(!screen.InitSuccessful() || !inputhandler.InitSuccessful()){
        std::cerr << "Failed to Initialize SDL2 Screen or InputHandler" << std::endl;
        return -1;
    }
    
    // Move to game logic type file / class later
    inputhandler.RegisterCallback(SDLK_ESCAPE, exit_callback);
    inputhandler.RegisterCallback(SDLK_c, toggle_culling_callback);
    inputhandler.RegisterCallback(SDLK_w, camera_forward);
    inputhandler.RegisterCallback(SDLK_a, camera_strafe_left);
    inputhandler.RegisterCallback(SDLK_s, camera_backward);
    inputhandler.RegisterCallback(SDLK_d, camera_strafe_right);
    inputhandler.RegisterCallback(SDLK_q, camera_rotate_left_big);
    inputhandler.RegisterCallback(SDLK_e, camera_rotate_right_big);
    inputhandler.RegisterCallback(SDLK_r, camera_rotate_up_big);
    inputhandler.RegisterCallback(SDLK_f, camera_rotate_down_big);
    inputhandler.RegisterMouseMoveCallback(InputHandler::MouseMove::MOVE_DOWN, camera_rotate_up);
    inputhandler.RegisterMouseMoveCallback(InputHandler::MouseMove::MOVE_UP, camera_rotate_down);
    inputhandler.RegisterMouseMoveCallback(InputHandler::MouseMove::MOVE_RIGHT, camera_rotate_right);
    inputhandler.RegisterMouseMoveCallback(InputHandler::MouseMove::MOVE_LEFT, camera_rotate_left);

    Scene scene = Scene(SceneSelection::TestScene);
    const std::list<Object3D> &obj_list = scene.get_obj_list_ref();
    const std::vector<std::unique_ptr<Light>> &light_vec = scene.get_light_vec_ref();

    // std::vector<Light> light_list; 

    // PointLight point_light;
    // point_light.position = Vector3(0, 2, 0);
    // point_light.ambient_intensity = 0.5;
    // point_light.diffuse_intensity = 2.0;
    // point_light.specular_intensity = 0.25;
    // light_list.push_back(point_light);

    // SpotLight spot_light;
    // spot_light.position = Vector3(0, 2, 0);
    // spot_light.ambient_intensity = 0.5;
    // spot_light.diffuse_intensity = 2.0;
    // spot_light.specular_intensity = 0.25;
    // spot_light.spotlight_direction_normal = Vector3(0, -1, 0);
    // spot_light.min_alignment = 0.5;
    // light_list.push_back(spot_light);

    // float rot = 0;

    while(gamestate.running){
        // Put frame time management in own function or object
        gamestate.WaitForFrame();
        float delta_time = gamestate.delta_time;
           
        std::vector<Triangle> rendered_triangles;

        // rot = 1 * delta_time;
        // point_light.position = Vector3(2*cos(rot), 2, 2*sin(rot));
        // rot += 1 * delta_time;
        
        // spot_light.rotate_spotlight(0, 0, rot);
        for(const auto &obj3d : obj_list){
            rendered_triangles.clear();

            // obj3d.position = Vector3(2*cos(rot), 2, 2*sin(rot));

            const Mesh &mesh = obj3d.m;
            const Texture &tex = obj3d.t;

            //TODO: Execute per frame obj updates here
            // obj3d.rotation.y += 0.7 * delta_time;
            // obj3d.rotation.z += 0.7 * delta_time;

            // This loop is essentially the "Vertex Shader" of my renderer
            for (auto i = 0; i < mesh.num_triangles; i++){
                const Face &f = mesh.faces[i];

                // Vertices are in (Homogenous) Model Space
                Triangle t{f, mesh};
                    
                // Vertices Are Transformed in World Spaace
                Matrix4x4 world_matrix = Matrix4x4::Identity();
                world_matrix = Matrix4x4::Scale(obj3d.scale.x, obj3d.scale.y, obj3d.scale.z) * world_matrix;

                // Seperate from world matrix as we use to rotate face and vertex normals
                Matrix4x4 rotation_matrix = Matrix4x4::Identity();

                rotation_matrix = Matrix4x4::XRotationMatrix(obj3d.rotation.x) * rotation_matrix;
                rotation_matrix = Matrix4x4::YRotationMatrix(obj3d.rotation.y) * rotation_matrix;
                rotation_matrix = Matrix4x4::ZRotationMatrix(obj3d.rotation.z) * rotation_matrix;

                world_matrix = rotation_matrix * world_matrix;

                world_matrix = Matrix4x4::Translation(obj3d.position.x, obj3d.position.y, obj3d.position.z) * world_matrix;

                t.MapVerts(world_matrix);

                // Add View Matrix support
                Matrix4x4 view_matrix = Matrix4x4::ViewMatrix(camera.position, camera.target, camera.up);
                t.MapVerts(view_matrix);

                if(gamestate.backface_culling_enabled && cull::should_backface_cull(t.a, t.b, t.c, {0, 0, 0})){
                    continue;
                }

                t.TransformInterpolants(rotation_matrix, world_matrix);
                t.vert_interp_a.vertex_color = t.a;
                t.vert_interp_b.vertex_color = t.b;
                t.vert_interp_c.vertex_color = t.c;

                //Flat Lighting Pass
                if(obj3d.light_type == LightingType::FLAT_LIGHTING){
                    // Calculate the normal for lighting calculation
                    float intensity = 0.0;

                    for (auto const &l : light_vec){
                        Vector3 face_normal = rotation_matrix * mesh.face_normals[i];
                        Vector3 face_midpoint = ((float)1/(float)3) * (t.vert_interp_a.vertex_position + t.vert_interp_b.vertex_position + t.vert_interp_c.vertex_position);
                        intensity += l->calculate_intensity(face_midpoint, face_normal, camera.position, obj3d.tex_params);
                    }

                    t.flat_shading_intensity = intensity;
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

                if(obj3d.render_type == RenderType::TEXTURED || obj3d.render_type == RenderType::TEXTURED_WIREFRAME){
                    draw.DrawFilledTriangle(t, obj3d, light_vec, camera.position);
                }

                if(obj3d.render_type == RenderType::WIREFRAME || obj3d.render_type == RenderType::TEXTURED_WIREFRAME){
                    draw.DrawTriangle(t.a, t.b, t.c, 0x00FFFFFF);
                }               
            }
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
