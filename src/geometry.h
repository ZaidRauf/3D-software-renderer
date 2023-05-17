#pragma once

#include "obj_loader.h"
#include <string>
#include <memory>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <map>
#include "linalg.h"

// Represents a 3d Triangular Face of a mesh
class Face{
    public:
        int a, b, c;
        int uv_a, uv_b, uv_c;
        Face();
        Face(int a, int b, int c);
        Face(int a, int b, int c, int uv_a, int uv_b, int uv_c);
        ~Face();
};

struct Transform{
    Transform();
    ~Transform();
    Vector3 scale;
    Vector3 translation;
    Vector3 rotation;
};

// The Full Triangular Mesh Class
class Mesh{
    private:
        int _num_triangles = 0;
        int _num_vertices = 0;
        void calculate_and_set_face_vertex_normals();

    public:
        std::unique_ptr<Face[]> faces;
        std::unique_ptr<Vector3[]> face_normals;

        std::unique_ptr<Vector3[]> vertices;
        std::unique_ptr<Vector3[]> vertex_normals;

        std::unique_ptr<Vector2[]> uv_coords;

        enum DefaultMesh { Cube, Triangle, Bunny };
        const int &num_triangles = _num_triangles;
        const int &num_vertices = _num_vertices;
        Transform transform;
        Mesh();
        Mesh(DefaultMesh meshEnum);
        Mesh(const std::string &filename);
        ~Mesh();
};

// Represents a 2D triangle projected onto the screen
class Triangle{
    public:
        Vector4 a, b, c;
        Vector2 uv_a, uv_b, uv_c;
        float flat_shading_intensity;
        uint32_t color = 0xFFFFFFFF;
        Triangle(Vector4 v1, Vector4 v2, Vector4 v3);
        Triangle(Vector4 v1, Vector4 v2, Vector4 v3, Vector2 uv_a, Vector2 uv_b, Vector2 uv_c);
        Triangle(const Face &face, const Mesh &mesh);
        ~Triangle();
        void MapVerts(const Matrix4x4 &m);
        void TransformInterpolants(const Matrix4x4 &rotation_mtx, const Matrix4x4 &world_mtx);

        struct VertexInterpolants {
            VertexInterpolants();
            VertexInterpolants(Vector3 pos, Vector3 color, Vector3 normal, Vector2 uv);
            ~VertexInterpolants();
            Vector3 vertex_position;
            Vector3 vertex_color;
            Vector3 vertex_normal;
            Vector2 vertex_uv;
            VertexInterpolants operator=(const VertexInterpolants &other);
        };
        using VertexInterpolants = struct VertexInterpolants;

        VertexInterpolants vert_interp_a, vert_interp_b, vert_interp_c;
};