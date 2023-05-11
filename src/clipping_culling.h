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
    void clip_vertices_uvs(const Triangle &t, std::vector<Vector4> &keep_vertex_list, std::vector<Vector2> &keep_uv_list);
    void clip_triangle(const Triangle &t, std::vector<Triangle> &keep_triangle_list);
    void retriangulate_clipped_vertices(const Triangle &orig_tri, std::vector<Vector4> &clipped_vertex_list, std::vector<Triangle> &rebuilt_triangles);
    void retriangulate_clipped_vertices_uvs(const Triangle &orig_tri, std::vector<Vector4> &clipped_vertex_list, std::vector<Vector2> &clipped_uv_list, std::vector<Triangle> &rebuilt_triangles);

}
