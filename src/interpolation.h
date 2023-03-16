#pragma once

#include "linalg.h"

namespace interpolation{
    template <class T>
    T lerp(const T &from, const T &to, const float &factor){
        T lerp_result = from + (factor * (to - from));
        return lerp_result;
    }
}
