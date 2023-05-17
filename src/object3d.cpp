
#include "object3d.h"

Object3D::Object3D(const Mesh &mesh, const Texture &tex) : m(mesh), t(tex){
    object_enabled = true;
    light_type = LightingType::FULL_BRIGHT;

    position = Vector3();
    rotation = Vector3();
}

Object3D::Object3D(const Mesh &mesh, const Texture &tex, LightingType light_type) : m(mesh), t(tex), light_type(light_type){
    object_enabled = true;
    position = Vector3();
    rotation = Vector3();
}

Object3D::Object3D(const Object3D &other) : m(other.m), t(other.t), light_type(other.light_type), object_enabled(other.object_enabled), position(other.position), rotation(other.rotation){
}

Object3D::~Object3D(){}