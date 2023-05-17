#pragma once

#include "linalg.h"

enum LightingType{
    FULL_BRIGHT,
    FLAT_LIGHTING,
    PHONG_LIGHTING
};

class Light {
    public:
        Light();
        ~Light();
        Vector3 color;
        Vector3 position;
        float intensity_specular;
        float intensity_diffuse;
};

class PointLight : private Light {
    public:
        PointLight(){
            color = Vector3();
        };
        ~PointLight();
};

class SpotLight : public Light {
    public:
        SpotLight(){
            color = Vector3();
        };
        ~SpotLight();
};