#pragma once

#include "linalg.h"
#include "light.h"
#include "geometry.h"
#include "texture.h"

enum RenderType {
    TEXTURED,
    WIREFRAME,
    TEXTURED_WIREFRAME
};

struct Object3D {
    Object3D(const Mesh &mesh, const Texture &tex);
    Object3D(const Object3D &other);

    ~Object3D();
    bool object_enabled;
    bool perspective_correct;
    const Mesh &m;
    const Texture &t;
    LightingType light_type;
    RenderType render_type;
    Vector3 position;
    Vector3 rotation;
};