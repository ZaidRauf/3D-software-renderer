#pragma once

#include "linalg.h"
#include "interpolation.h"
#include "geometry.h"
#include <cstdbool>
#include <vector>
#include <array>
#include <utility>

namespace cull{
    bool should_backface_cull(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, const Vector3 &viewPos);
}

namespace clip{
    void clip_vertices(Vector4 v1, Vector4 v2, Vector4 v3, std::vector<Vector4> &keep_vertex_list);
    void retriangulate_clipped_vertices(std::vector<Vector4> &clipped_vertex_list, std::vector<Triangle> &rebuilt_triangles);
}
