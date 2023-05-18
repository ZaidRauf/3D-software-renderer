
#include "object3d.h"

Object3D::Object3D(const Mesh &mesh, const Texture &tex) : m(mesh), t(tex){
    object_enabled = true;
    perspective_correct = true;
    light_type = LightingType::PHONG_SHADING;
    render_type = RenderType::TEXTURED;
    position = Vector3();
    rotation = Vector3();
    scale = Vector3(1, 1, 1);
    tex_params = TextureParameters();
    obj_id = 0;
}

Object3D::Object3D(const Object3D &other) : m(other.m), t(other.t), light_type(other.light_type), object_enabled(other.object_enabled), position(other.position), rotation(other.rotation){
    perspective_correct = other.perspective_correct;
    render_type = other.render_type;
    tex_params = other.tex_params;
    scale = other.scale;
    obj_id = other.obj_id;
}

Object3D::~Object3D(){}

TextureParameters::TextureParameters(){
    specular_coeffieicent = 1.0;
    diffuse_coefficient = 1.0;
    ambient_coefficient = 1.0;
    alpha = 1.0;
}

TextureParameters::TextureParameters(float spec, float diff, float ambi, float alph){
     specular_coeffieicent = spec;
     diffuse_coefficient = diff;
     ambient_coefficient = ambi;
     alpha = alph;
}

TextureParameters::TextureParameters(const TextureParameters &other){
    specular_coeffieicent = other.specular_coeffieicent;
    diffuse_coefficient = other.diffuse_coefficient;
    ambient_coefficient = other.ambient_coefficient;
    alpha = other.alpha;
}

TextureParameters::~TextureParameters(){}