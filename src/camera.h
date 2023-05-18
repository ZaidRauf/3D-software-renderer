#pragma once

#include "linalg.h"

class Camera{
    public:
        Vector3 position;
        Vector3 target;
        Vector3 up;
        Vector3 forward;
        Vector3 side;
        Camera(const Vector3 &position, const Vector3 &target, const Vector3 &up);
        ~Camera();
        void camera_forward();
        void camera_backward();
        void camera_strafe_left();
        void camera_strafe_right();
        void camera_rotate_right();
        void camera_rotate_left();
};