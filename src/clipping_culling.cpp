#include "clipping_culling.h"

bool cull::should_backface_cull(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, const Vector3 &viewPos){
    // Back face culling
    
    // First get the Face Normal
    // Be wary of winding order and handedness here
    // since this is a left handed coordinate system with clockwise winding order keep that in mind for cross product
    Vector3 normal = Vector3::Cross(v1 - v2, v1 - v3).Normalized();
    
    // Calculate ray from face to camera
    Vector3 face_view_ray = (viewPos - Vector3(v1)).Normalized();
    
    if(Vector3::Dot(face_view_ray, normal) <= 0){
        return true;
    }

    return false;
}

