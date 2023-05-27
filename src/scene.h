#pragma once

#include <vector>
#include <string>
#include "linalg.h"
#include "geometry.h"
#include "object3d.h"
#include <map>
#include <list>
#include "light.h"
#include <memory>
#include <cmath>

enum SceneSelection {
    Dolphin,
    Crate,
    PerspCorrect,
    Spheres,
    SCENE_COUNT = 4
};

class Scene {
    public:
        Scene(SceneSelection select);
        ~Scene();
        const std::vector<Object3D>& get_obj_list_ref() const;
        const std::vector<std::unique_ptr<Light>>& get_light_vec_ref() const;
        void perform_frame_update(float delta_time);
        void next_scene();
        Vector3 get_init_camera_pos();

    private:
        Vector3 init_camera_pos;
        SceneSelection selection;
        std::map<std::string, Mesh> mesh_map;
        std::map<std::string, Texture> tex_map;
        std::vector<Object3D> obj_list;
        std::vector<std::unique_ptr<Light>> light_vec; 
        void load_assets();
        void init_scene_objects();
        std::function<void(double)> frame_update_func;
};
