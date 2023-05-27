
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
        case TestScene:
            mesh_map.emplace("cube", "./assets/models/cube.obj");
            tex_map.emplace("crate", "./assets/textures/crate.tga");
            tex_map.emplace("test", "./assets/textures/test_cube_texture.tga");
            tex_map.emplace("gray", Texture::DefaultTexture::Gray);

        case Dolphin:
            mesh_map.emplace("cube", "./assets/models/cube.obj");
            mesh_map.emplace("cone", "./assets/models/cone.obj");
            mesh_map.emplace("dolphin", "./assets/models/dolphin.obj");

            tex_map.emplace("metal", "./assets/textures/metal.tga");
            tex_map.emplace("gray", Texture::DefaultTexture::Gray);

    }
}

void Scene::init_scene_objects(){
    auto PI = 3.14159;

    switch (selection){
        case TestScene:
            {
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

                auto frame_update = [=](float delta_time) {
                auto &crate = obj_list[0];
                auto &light_mdl = obj_list[1];

                auto &point = light_vec[0];
                auto &spot = light_vec[1];

                //   rot += 1 * delta_time;
                //   crate.rotation = Vector3(0, rot, 0);

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
                light_cube.scale = Vector3(0.15, 0.15, 0.15);
                obj_list.push_back(light_cube);

                auto point_light = std::make_unique<PointLight>();
                point_light->position = Vector3(0, 2, 0);
                point_light->ambient_intensity = 1.0;
                point_light->diffuse_intensity = 1.0;
                point_light->specular_intensity = 1.0;
                light_vec.push_back( std::move(point_light) );

                // Like this to ensure light rotation is reset if the scene is restarted
                static float light_rotation;
                light_rotation = 0.0;

                auto frame_update = [=](float delta_time) {
                    auto &light_mdl = obj_list[1];
                    auto &point = light_vec[0];

                    light_rotation += 1 * delta_time;
                    auto new_light_pos = Vector3(2*cos(light_rotation), point->position.y, 2*sin(light_rotation));
                    point->position = new_light_pos;
                    light_mdl.position = new_light_pos;
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