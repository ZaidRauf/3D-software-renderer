
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
}

void Scene::load_assets(){
    switch (selection){
        case TestScene:
            mesh_map.emplace("cube", "./assets/models/cube.obj");
            tex_map.emplace("crate", "./assets/textures/crate.tga");
            tex_map.emplace("test", "./assets/textures/test_cube_texture.tga");
            tex_map.emplace("gray", Texture::DefaultTexture::Gray);
    }
}

void Scene::init_scene_objects(){
    switch (selection){
        case TestScene:
            Object3D obj3d = Object3D(mesh_map.at("cube"), tex_map.at("crate"));
            obj3d.position = Vector3(0, 0, 0);
            obj3d.light_type = LightingType::PHONG_SHADING;
            obj3d.render_type = RenderType::TEXTURED;
            obj3d.perspective_correct = true;
            obj3d.obj_id = 1;
            obj_list.push_back(obj3d);
            
            Object3D obj3d3 = Object3D(mesh_map.at("cube"), tex_map.at("gray"));
            obj3d3.position = Vector3(0, 2, 0);
            obj3d3.light_type = LightingType::FLAT_LIGHTING;
            obj3d3.render_type = RenderType::TEXTURED;
            obj3d3.scale = Vector3(0.15, 0.15, 0.15);
            obj_list.push_back(obj3d3);

            auto point_light = std::make_unique<PointLight>();
            point_light->position = Vector3(0, 2, 0);
            point_light->ambient_intensity = 0.5;
            point_light->diffuse_intensity = 2.0;
            point_light->specular_intensity = 0.25;
            light_vec.push_back( std::move(point_light) );

            // auto spot_light = std::make_unique<SpotLight>();
            // spot_light->position = Vector3(0, 2, 0);
            // spot_light->ambient_intensity = 0.5;
            // spot_light->diffuse_intensity = 2.0;
            // spot_light->specular_intensity = 0.25;
            // spot_light->spotlight_direction_normal = Vector3(0, -1, 0);
            // spot_light->min_alignment = 0.5;
            // light_vec.push_back( std::move(spot_light) );

            // static float rot = 0.0;
            static float light_rotation = 0.0;
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
}

void Scene::perform_frame_update(float delta_time){
    frame_update_func(delta_time);
}

const std::vector<Object3D>& Scene::get_obj_list_ref(){
    return obj_list;
}

const std::vector<std::unique_ptr<Light>>& Scene::get_light_vec_ref(){
    return light_vec;
}
