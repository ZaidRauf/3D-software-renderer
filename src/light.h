#pragma once

#include "linalg.h"
#include <cmath>
#include "object3d.h"

class Light {
    public:
        Light();
        virtual ~Light();
        virtual float calculate_intensity(const Vector3 &face_point, const Vector3 &face_normal, const Vector3 &camera_position, const TextureParameters &tex_params) const = 0;
        Vector3 position;
        float ambient_intensity;
        float diffuse_intensity;
        float specular_intensity;
};


class PointLight : public Light {
    public:
        PointLight();
        ~PointLight();
        float calculate_intensity(const Vector3 &face_point, const Vector3 &face_normal, const Vector3 &camera_position, const TextureParameters &tex_params) const;
};

// class SpotLight : public Light {
//     public:
//         SpotLight();
//         ~SpotLight();
// };