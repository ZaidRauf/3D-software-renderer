
#include "object3d.h"

Object3D::Object3D(const Mesh &mesh, const Texture &tex) : m(mesh), t(tex){
    object_enabled = true;
    perspective_correct = true;
    light_type = LightingType::PHONG_LIGHTING;
    render_type = RenderType::TEXTURED;
    position = Vector3();
    rotation = Vector3();
}

Object3D::Object3D(const Object3D &other) : m(other.m), t(other.t), light_type(other.light_type), object_enabled(other.object_enabled), position(other.position), rotation(other.rotation){
    perspective_correct = other.perspective_correct;
    render_type = other.render_type;
}

Object3D::~Object3D(){}