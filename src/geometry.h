#pragma once

#include <memory>
#include "linalg.h"

// Represents a 2D triangle projected onto the screen
class Triangle{
    public:
        Vector2 a, b, c;
        Triangle(Vector2 v1, Vector2 v2, Vector3 v3);
        ~Triangle();
};

// Represents a 3d Triangular Face of a mesh
class Face{
    public:
        int a, b, c;
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
        std::unique_ptr<Face[]> faces;
        std::unique_ptr<Vector3[]> vertices;
        int _num_triangles = 0;
    
    public:
        enum DefaultMesh { Cube };
        const int &num_triangles = _num_triangles;
        Transform transform;
        Mesh();
        Mesh(DefaultMesh meshEnum);
        ~Mesh();
};

