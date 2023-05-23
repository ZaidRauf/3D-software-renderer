#pragma once

#include "linalg.h"
#include "geometry.h"
#include "texture.h"

enum RenderType {
    TEXTURED,
    WIREFRAME,
    TEXTURED_WIREFRAME
};

enum LightingType{
    FULL_BRIGHT,
    FLAT_LIGHTING,
    PHONG_SHADING
};

struct Object3D {
    struct TextureParameters{
        TextureParameters();
        TextureParameters(float spec, float diff, float ambi, float alph);
        TextureParameters(const TextureParameters &other);
        ~TextureParameters();

        float specular_coeffieicent;
        float diffuse_coefficient;
        float ambient_coefficient;
        float alpha;
    };
    using TextureParameters = struct TextureParameters;

    Object3D(const Mesh &mesh, const Texture &tex);
    Object3D(const Object3D &other);
    ~Object3D();
    int obj_id;
    bool object_enabled;
    bool backface_culling_enabled;
    bool perspective_correct;
    const Mesh &m;
    const Texture &t;
    LightingType light_type;
    RenderType render_type;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    TextureParameters tex_params;
};

using TextureParameters = Object3D::TextureParameters;