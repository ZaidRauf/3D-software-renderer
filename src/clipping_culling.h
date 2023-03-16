#pragma once

#include "linalg.h"
#include <cstdbool>

namespace cull{
    bool should_backface_cull(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, const Vector3 &viewPos);
}
