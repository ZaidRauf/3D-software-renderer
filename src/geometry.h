#pragma once

#include <memory>
#include <cstdint>
#include "linalg.h"

// Represents a 2D triangle projected onto the screen
class Triangle{
    public:
        Vector4 a, b, c;
        Vector2 uv_a, uv_b, uv_c;
        uint32_t color = 0xFFFFFFFF;
        Triangle(Vector4 v1, Vector4 v2, Vector4 v3);
        Triangle(Vector4 v1, Vector4 v2, Vector4 v3, Vector2 uv_a, Vector2 uv_b, Vector2 uv_c);
        ~Triangle();
        void MapVerts(const Matrix4x4 &m);
};

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
    
    public:
        std::unique_ptr<Face[]> faces;
        std::unique_ptr<Vector3[]> vertices;
        std::unique_ptr<Vector2[]> uv_coords;
        enum DefaultMesh { Cube, Triangle, Bunny };
        const int &num_triangles = _num_triangles;
        const int &num_vertices = _num_vertices;
        Transform transform;
        Mesh();
        Mesh(DefaultMesh meshEnum);
        ~Mesh();
};

