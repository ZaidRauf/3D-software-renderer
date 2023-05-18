#include "light.h"

Light::Light(){};

Light::~Light(){};

// float Light::calculate_intensity(){
//     return 1.0f;
// }


PointLight::PointLight(){
    ambient_intensity = 1.0;
    diffuse_intensity = 1.0;
    specular_intensity = 1.0;
};

PointLight::~PointLight(){};

float PointLight::calculate_intensity(const Vector3 &face_point, const Vector3 &face_normal, const Vector3 &camera_position, const TextureParameters &tex_params) const{
    Vector3 light_dir_normal = (this->position - face_point).Normalized();
    Vector3 camera_dir_normal = (camera_position - face_point).Normalized();
    Vector3 reflected_dir_normal = (2*(light_dir_normal * face_normal)*face_normal) - light_dir_normal;

    float light_fall_off = 1/sqrt((this->position - face_point).Length());

    float intensity = (ambient_intensity * tex_params.ambient_coefficient) +
    (light_fall_off * diffuse_intensity * tex_params.diffuse_coefficient * std::max((float)0, face_normal * light_dir_normal)) +
    (specular_intensity * tex_params.specular_coeffieicent * pow(std::max((float)0, reflected_dir_normal * camera_dir_normal), tex_params.alpha));

    return intensity;
}