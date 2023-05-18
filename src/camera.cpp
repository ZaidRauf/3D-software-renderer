#include "camera.h"

Camera::Camera(const Vector3 &position, const Vector3 &target, const Vector3 &up) : position(position), target(target), up(up){
    forward = (target - position).Normalized();
    side = Vector3::Cross(forward, up).Normalized();
    this->target = position + forward;
}

Camera::~Camera() {};

void Camera::camera_forward() {
    position = position + (forward * 0.1);
    target = target + (forward * 0.1);
}

void Camera::camera_backward() {
    position = position - (forward * 0.1);
    target = target - (forward * 0.1);
}

void Camera::camera_strafe_left() {
    position = position + (side * 0.1);
    target = target + (side * 0.1);
}

void Camera::camera_strafe_right() {
    position = position - (side * 0.1);
    target = target - (side * 0.1);
}

void Camera::camera_rotate_right() {
    forward = Matrix4x4::YRotationMatrix(0.1) * forward;
    target = position + forward;
    side = Vector3::Cross(forward, up).Normalized();
}

void Camera::camera_rotate_left() {
    forward = Matrix4x4::YRotationMatrix(-0.1) * forward;
    target = position + forward;
    side = Vector3::Cross(forward, up).Normalized();
}

void Camera::camera_rotate_up() {
    forward = Matrix4x4::AxisAngleRotationMatrix(side, 0.1) * forward;
    target = position + forward;
    // side = Vector3::Cross(forward, up).Normalized();
}

void Camera::camera_rotate_down() {
    forward = Matrix4x4::AxisAngleRotationMatrix(side, -0.1) * forward;
    target = position + forward;
    // side = Vector3::Cross(forward, up).Normalized();
}
