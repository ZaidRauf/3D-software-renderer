#pragma once

#include "linalg.h"
#include <cmath>

namespace interpolation{
    template <class T>
    T lerp(const T &from, const T &to, const float &factor){
        T lerp_result = from + (factor * (to - from));
        return lerp_result;
    }

    Vector3 barycentric_weights(const Vector2 &a, const Vector2 &b, const Vector2 &c, const Vector2 &p);
}
