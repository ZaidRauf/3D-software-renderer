#pragma once

#include <memory>
#include <cstdint>
#include "linalg.h"

// Represents a 2D triangle projected onto the screen
class Triangle{
    public:
        Vector2 a, b, c;
        Triangle(Vector2 v1, Vector2 v2, Vector2 v3);
        ~Triangle();
};

// Represents a 3d Triangular Face of a mesh
class Face{
    public:
        int a, b, c;
        uint32_t color;
        Face();
        Face(int a, int b, int c);
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
    
    public:
        std::unique_ptr<Face[]> faces;
        std::unique_ptr<Vector3[]> vertices;
        enum DefaultMesh { Cube };
        const int &num_triangles = _num_triangles;
        Transform transform;
        Mesh();
        Mesh(DefaultMesh meshEnum);
        ~Mesh();
};

