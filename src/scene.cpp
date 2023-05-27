
#include "scene.h"

Scene::Scene(SceneSelection select) : selection(select){
    load_assets();
    init_scene_objects();
}

Scene::~Scene(){
    // Remove all references to textures and models
    while(!obj_list.empty()){
        obj_list.pop_back();
    }

    obj_list.clear();
    tex_map.clear();
    mesh_map.clear();
    light_vec.clear();
}

void Scene::load_assets(){
    switch (selection){
        case Crate:
            mesh_map.emplace("cube", "./assets/models/cube.obj");
            
            tex_map.emplace("crate", "./assets/textures/crate.tga");
            tex_map.emplace("gray", Texture::DefaultTexture::Gray);

        case Dolphin:
            mesh_map.emplace("cube", "./assets/models/cube.obj");
            mesh_map.emplace("cone", "./assets/models/cone.obj");
            mesh_map.emplace("dolphin", "./assets/models/dolphin.obj");

            tex_map.emplace("metal", "./assets/textures/metal.tga");
            tex_map.emplace("gray", Texture::DefaultTexture::Gray);

        case PerspCorrect:
            mesh_map.emplace("cube", "./assets/models/cube.obj");
            tex_map.emplace("test", "./assets/textures/test_cube_texture.tga");

        case Spheres:
            mesh_map.emplace("sphere", "./assets/models/sphere.obj");
            mesh_map.emplace("cube", "./assets/models/cube.obj");
            mesh_map.emplace("cone", "./assets/models/cone.obj");

            tex_map.emplace("yinyang", "./assets/textures/yin_yang.tga");
            tex_map.emplace("gray", Texture::DefaultTexture::Gray);
    }
}

void Scene::init_scene_objects(){
    float PI = 3.14159;

    switch (selection){
        case Spheres:
           { 
                init_camera_pos = Vector3(0, 0, -3);

                Object3D sphere1 = Object3D(mesh_map.at("sphere"), tex_map.at("yinyang"));
                sphere1.scale = Vector3(0.5, 0.5, 0.5);
                sphere1.position = Vector3(0.75, -0.5, 0);
                sphere1.rotation = Vector3(0, -PI/2, 0);
                sphere1.light_type = LightingType::PHONG_SHADING;
                sphere1.render_type = RenderType::TEXTURED;
                sphere1.perspective_correct = true;
                sphere1.obj_id = 1;
                sphere1.tex_params.ambient_coefficient = 0.5;
                sphere1.tex_params.diffuse_coefficient = 1.5;
                sphere1.tex_params.specular_coeffieicent = 5.0;
                sphere1.tex_params.alpha = 10;
                obj_list.push_back(sphere1);

                Object3D sphere2 = Object3D(mesh_map.at("sphere"), tex_map.at("yinyang"));
                sphere2.scale = Vector3(0.5, 0.5, 0.5);
                sphere2.position = Vector3(-0.75, -0.5, 0);
                sphere2.rotation = Vector3(0, -PI/2, 0);
                sphere2.light_type = LightingType::FLAT_LIGHTING;
                sphere2.render_type = RenderType::TEXTURED;
                sphere2.perspective_correct = true;
                sphere2.obj_id = 1;
                sphere2.tex_params.ambient_coefficient = 0.5;
                sphere2.tex_params.diffuse_coefficient = 1.5;
                sphere2.tex_params.specular_coeffieicent = 5.0;
                sphere2.tex_params.alpha = 10;
                obj_list.push_back(sphere2);

                Object3D light_cube = Object3D(mesh_map.at("cube"), tex_map.at("gray"));
                light_cube.position = Vector3(0, 2, 0);
                light_cube.light_type = LightingType::FLAT_LIGHTING;
                light_cube.render_type = RenderType::TEXTURED;
                light_cube.tex_params.ambient_coefficient = 0.5;
                light_cube.tex_params.diffuse_coefficient = 0.5;
                light_cube.tex_params.specular_coeffieicent = 0.0;
                light_cube.scale = Vector3(0.15, 0.15, 0.15);
                obj_list.push_back(light_cube);

                Object3D light_cone = Object3D(mesh_map.at("cone"), tex_map.at("gray"));
                light_cone.position = Vector3(0, 2, 0);
                light_cone.light_type = LightingType::FLAT_LIGHTING;
                light_cone.render_type = RenderType::TEXTURED;
                light_cone.tex_params.ambient_coefficient = 0.1;
                light_cone.tex_params.diffuse_coefficient = 0.25;
                light_cone.tex_params.specular_coeffieicent = 0.0;
                light_cone.scale = Vector3(0.25, 0.25, 0.25);
                obj_list.push_back(light_cone);

                auto point_light = std::make_unique<PointLight>();
                point_light->position = Vector3(0, 2, 0);
                point_light->ambient_intensity = 0.25;
                point_light->diffuse_intensity = 1.0;
                point_light->specular_intensity = 1.0;
                light_vec.push_back( std::move(point_light) );

                auto spot_light = std::make_unique<SpotLight>();
                spot_light->position = Vector3(0, 2, 0);
                spot_light->ambient_intensity = 1.0;
                spot_light->diffuse_intensity = 1.0;
                spot_light->specular_intensity = 1.0;
                spot_light->spotlight_direction_normal = Vector3(0, -1, 0);
                spot_light->min_alignment = 0.5;
                light_vec.push_back( std::move(spot_light) );

                // Like this to ensure light rotation is reset if the scene is restarted
                static float point_light_rotation;
                point_light_rotation = 0.0;

                static float cube_light_rotation;
                cube_light_rotation = 0.0;

                static float cone_light_rotation;
                static int cone_light_rot_dir;
                cone_light_rot_dir = 1;
                cone_light_rotation = 0.0;

                auto frame_update = [=](float delta_time) {
                    auto &cube_light_mdl = obj_list[2];
                    auto &point = light_vec[0];
                    auto &spot = light_vec[1];

                    point_light_rotation += 1 * delta_time;
                    auto new_light_pos = Vector3(2*cos(point_light_rotation), point->position.y, 2*sin(point_light_rotation));
                    point->position = new_light_pos;
                    cube_light_mdl.position = new_light_pos;
                    

                    if(std::abs(cone_light_rotation) >= PI/2){
                        cone_light_rot_dir *= -1;
                    }

                    cone_light_rotation += cone_light_rot_dir * delta_time;
                    auto delta_rot_mtx = Matrix4x4::ZRotationMatrix(cone_light_rot_dir * delta_time);
                    auto cast_spot = dynamic_cast<SpotLight*>(spot.get());
                    cast_spot->spotlight_direction_normal = delta_rot_mtx * cast_spot->spotlight_direction_normal;
                    auto &cone_light_mdl = obj_list[3];
                    cone_light_mdl.rotation = Vector3(0, 0, cone_light_rotation);
                };

                frame_update_func = frame_update;
           }
           break;
        case PerspCorrect:
            {
                init_camera_pos = Vector3(0, 0, -5);

                Object3D persp_incorrect_cube = Object3D(mesh_map.at("cube"), tex_map.at("test"));
                persp_incorrect_cube.position = Vector3(-2, 0, 0);
                persp_incorrect_cube.light_type = LightingType::FULL_BRIGHT;
                persp_incorrect_cube.render_type = RenderType::TEXTURED;
                persp_incorrect_cube.perspective_correct = false;
                obj_list.push_back(persp_incorrect_cube);

                Object3D persp_correct_cube = Object3D(mesh_map.at("cube"), tex_map.at("test"));
                persp_correct_cube.position = Vector3(2, 0, 0);
                persp_correct_cube.light_type = LightingType::FULL_BRIGHT;
                persp_correct_cube.render_type = RenderType::TEXTURED;
                persp_correct_cube.perspective_correct = true;
                obj_list.push_back(persp_correct_cube);
                
                static float cube_rotation;
                cube_rotation = 0.0;

                auto frame_update = [=](float delta_time) {
                    auto &cube1 = obj_list[0];
                    auto &cube2 = obj_list[1];

                      cube_rotation += 1 * delta_time;
                      cube1.rotation = Vector3(0, cube_rotation, cube_rotation);
                      cube2.rotation = Vector3(0, cube_rotation, cube_rotation);
                };

                frame_update_func = frame_update;
            }
            break;
        case Crate:
            {
                init_camera_pos = Vector3(0, 0, -5);
                Object3D crate = Object3D(mesh_map.at("cube"), tex_map.at("crate"));
                crate.position = Vector3(0, 0, 0);
                crate.light_type = LightingType::PHONG_SHADING;
                crate.render_type = RenderType::TEXTURED;
                crate.perspective_correct = true;
                crate.obj_id = 1;
                obj_list.push_back(crate);
                
                Object3D light_cube = Object3D(mesh_map.at("cube"), tex_map.at("gray"));
                light_cube.position = Vector3(0, 2, 0);
                light_cube.light_type = LightingType::FLAT_LIGHTING;
                light_cube.render_type = RenderType::TEXTURED;
                light_cube.scale = Vector3(0.15, 0.15, 0.15);
                obj_list.push_back(light_cube);

                auto point_light = std::make_unique<PointLight>();
                point_light->position = Vector3(0, 2, 0);
                point_light->ambient_intensity = 0.5;
                point_light->diffuse_intensity = 2.0;
                point_light->specular_intensity = 0.25;
                light_vec.push_back( std::move(point_light) );
                
                // Like this to ensure light rotation is reset if the scene is restarted
                static float light_rotation;
                light_rotation = 0.0;

                static float crate_rotation;
                crate_rotation = 0.0;

                auto frame_update = [=](float delta_time) {
                    auto &crate = obj_list[0];
                    auto &light_mdl = obj_list[1];

                    auto &point = light_vec[0];
                    auto &spot = light_vec[1];

                      crate_rotation += 1 * delta_time;
                      crate.rotation = Vector3(0, crate_rotation, crate_rotation);

                    light_rotation += 1 * delta_time;
                    auto new_light_pos = Vector3(3*cos(light_rotation), point->position.y, 3*sin(light_rotation));
                    point->position = new_light_pos;
                    light_mdl.position = new_light_pos;
                };

                frame_update_func = frame_update;
            }
            break;
        case Dolphin:
           { 
                init_camera_pos = Vector3(0, 0, -1);
                Object3D dolphin = Object3D(mesh_map.at("dolphin"), tex_map.at("metal"));
                dolphin.position = Vector3(0, 0, 0);
                dolphin.rotation = Vector3(0, 5*PI / 4, 0);
                dolphin.light_type = LightingType::PHONG_SHADING;
                dolphin.render_type = RenderType::TEXTURED;
                dolphin.perspective_correct = true;
                dolphin.obj_id = 1;
                dolphin.tex_params.ambient_coefficient = 0.5;
                dolphin.tex_params.diffuse_coefficient = 1.5;
                dolphin.tex_params.specular_coeffieicent = 5.0;
                dolphin.tex_params.alpha = 10;
                obj_list.push_back(dolphin);

                Object3D light_cube = Object3D(mesh_map.at("cube"), tex_map.at("gray"));
                light_cube.position = Vector3(0, 2, 0);
                light_cube.light_type = LightingType::FLAT_LIGHTING;
                light_cube.render_type = RenderType::TEXTURED;
                light_cube.tex_params.ambient_coefficient = 0.5;
                light_cube.tex_params.diffuse_coefficient = 0.5;
                light_cube.tex_params.specular_coeffieicent = 0.0;
                light_cube.scale = Vector3(0.15, 0.15, 0.15);
                obj_list.push_back(light_cube);

                Object3D light_cone = Object3D(mesh_map.at("cone"), tex_map.at("gray"));
                light_cone.position = Vector3(0, 2, 0);
                light_cone.light_type = LightingType::FLAT_LIGHTING;
                light_cone.render_type = RenderType::TEXTURED;
                light_cone.tex_params.ambient_coefficient = 0.1;
                light_cone.tex_params.diffuse_coefficient = 0.25;
                light_cone.tex_params.specular_coeffieicent = 0.0;
                light_cone.scale = Vector3(0.25, 0.25, 0.25);
                obj_list.push_back(light_cone);

                auto point_light = std::make_unique<PointLight>();
                point_light->position = Vector3(0, 2, 0);
                point_light->ambient_intensity = 1.0;
                point_light->diffuse_intensity = 1.0;
                point_light->specular_intensity = 1.0;
                light_vec.push_back( std::move(point_light) );

                auto spot_light = std::make_unique<SpotLight>();
                spot_light->position = Vector3(0, 2, 0);
                spot_light->ambient_intensity = 0.0;
                spot_light->diffuse_intensity = 1.0;
                spot_light->specular_intensity = 1.0;
                spot_light->spotlight_direction_normal = Vector3(0, -1, 0);
                spot_light->min_alignment = 0.5;
                light_vec.push_back( std::move(spot_light) );

                // Like this to ensure light rotation is reset if the scene is restarted
                static float point_light_rotation;
                point_light_rotation = 0.0;

                static float cube_light_rotation;
                cube_light_rotation = 0.0;

                static float cone_light_rotation;
                static int cone_light_rot_dir;
                cone_light_rot_dir = 1;
                cone_light_rotation = 0.0;

                auto frame_update = [=](float delta_time) {
                    auto &cube_light_mdl = obj_list[1];
                    auto &point = light_vec[0];
                    auto &spot = light_vec[1];

                    point_light_rotation += 1 * delta_time;
                    auto new_light_pos = Vector3(2*cos(point_light_rotation), point->position.y, 2*sin(point_light_rotation));
                    point->position = new_light_pos;
                    cube_light_mdl.position = new_light_pos;
                    

                    if(std::abs(cone_light_rotation) >= PI/2){
                        cone_light_rot_dir *= -1;
                    }

                    cone_light_rotation += cone_light_rot_dir * delta_time;
                    auto delta_rot_mtx = Matrix4x4::ZRotationMatrix(cone_light_rot_dir * delta_time);
                    auto cast_spot = dynamic_cast<SpotLight*>(spot.get());
                    cast_spot->spotlight_direction_normal = delta_rot_mtx * cast_spot->spotlight_direction_normal;
                    auto &cone_light_mdl = obj_list[2];
                    cone_light_mdl.rotation = Vector3(0, 0, cone_light_rotation);
                };

                frame_update_func = frame_update;
           }
           break;
        default:
            break;
    }
}

void Scene::perform_frame_update(float delta_time){
    frame_update_func(delta_time);
}

const std::vector<Object3D>& Scene::get_obj_list_ref() const{
    return obj_list;
}

const std::vector<std::unique_ptr<Light>>& Scene::get_light_vec_ref() const{
    return light_vec;
}

void Scene::next_scene(){
    // Remove all references to textures and models
    while(!obj_list.empty()){
        obj_list.pop_back();
    }

    obj_list.clear();
    tex_map.clear();
    mesh_map.clear();
    light_vec.clear();

    auto scene_num = (selection + 1) % SceneSelection::SCENE_COUNT;
    selection = static_cast<SceneSelection>(scene_num);

    frame_update_func = [=](float delta_time) {};

    load_assets();
    init_scene_objects();
}

Vector3 Scene::get_init_camera_pos() {
    return init_camera_pos;
}