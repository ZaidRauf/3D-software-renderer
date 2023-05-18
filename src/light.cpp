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

SpotLight::SpotLight(){
    ambient_intensity = 1.0;
    diffuse_intensity = 1.0;
    specular_intensity = 1.0;
    spotlight_direction_normal = Vector3(0, -1, 0);
    min_alignment = 0.0;
};

SpotLight::~SpotLight(){};

float SpotLight::calculate_intensity(const Vector3 &face_point, const Vector3 &face_normal, const Vector3 &camera_position, const TextureParameters &tex_params) const{
    Vector3 light_dir_normal = (this->position - face_point).Normalized();
    Vector3 camera_dir_normal = (camera_position - face_point).Normalized();
    Vector3 reflected_dir_normal = (2*(light_dir_normal * face_normal)*face_normal) - light_dir_normal;

    float light_fall_off = 1/sqrt((this->position - face_point).Length());

    float intensity = (ambient_intensity * tex_params.ambient_coefficient) +
    (light_fall_off * diffuse_intensity * tex_params.diffuse_coefficient * std::max((float)0, face_normal * light_dir_normal)) +
    (specular_intensity * tex_params.specular_coeffieicent * pow(std::max((float)0, reflected_dir_normal * camera_dir_normal), tex_params.alpha));
    
    float spotlight_centre_alignment = Vector3::Dot((-1) * light_dir_normal, spotlight_direction_normal);

    float scaled_alignment = std::clamp((float)((spotlight_centre_alignment - min_alignment)/(1.0 - min_alignment)), (float)0, (float)1.0);

    return intensity * scaled_alignment;
}

void SpotLight::rotate_spotlight(float x_rot, float y_rot, float z_rot){
    auto rotation_matrix = Matrix4x4::XRotationMatrix(x_rot);
    rotation_matrix = Matrix4x4::YRotationMatrix(y_rot) * rotation_matrix;
    rotation_matrix = Matrix4x4::ZRotationMatrix(z_rot) * rotation_matrix;
    spotlight_direction_normal = rotation_matrix * spotlight_direction_normal;
}
