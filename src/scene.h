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

enum SceneSelection {
    TestScene
};

class Scene {
    public:
        Scene(SceneSelection select);
        ~Scene();
        const std::list<Object3D>& get_obj_list_ref();
        const std::vector<std::unique_ptr<Light>>& get_light_vec_ref();

    private:
        SceneSelection selection;
        std::map<std::string, Mesh> mesh_map;
        std::map<std::string, Texture> tex_map;
        std::list<Object3D> obj_list;
        std::vector<std::unique_ptr<Light>> light_vec; 
        void load_assets();
        void init_scene_objects();
};
