#pragma once

#include "linalg.h"

class Camera{
    public:
        Vector3 position;
        Vector3 target;
        Vector3 orientation;
        Camera(const Vector3 &position, const Vector3 &target, const Vector3 &orientation);
};


