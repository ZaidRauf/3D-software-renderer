#include "geometry.h"


Triangle::Triangle(Vector2 v1, Vector2 v2, Vector2 v3) : a(v1), b(v2), c(v3){
}

Triangle::~Triangle(){}

Face::Face(){}

Face::Face(int a, int b, int c) : a(a), b(b), c(c){};

Face::~Face(){};

Transform::Transform(){};
Transform::~Transform(){};

Mesh::Mesh(){};
Mesh::~Mesh(){};

Mesh::Mesh(DefaultMesh meshEnum){
    if(meshEnum == Cube){
        faces = std::make_unique<Face[]>(12);
        vertices = std::make_unique<Vector3[]>(8);
        _num_triangles = 12;


        vertices[0] = {-1, -1, -1}; // 1
        vertices[1] = {-1, 1,  -1}; // 2
        vertices[2] = {1,  1,  -1}; // 3
        vertices[3] = {1,  -1, -1}; // 4
        vertices[4] = {1,  1,  1}; // 5
        vertices[5] = {1,  -1, 1}; // 6
        vertices[6] = {-1, 1,  1}; // 7
        vertices[7] = {-1, -1, 1};  // 8

        faces[0] = {1,2,3}; // 1
        faces[1] = {1,3,4}; // 2
        faces[2] = {4,3,5}; // 3
        faces[3] = {4,5,6}; // 4
        faces[4] = {6,5,7}; // 5
        faces[5] = {6,7,8}; // 6
        faces[6] = {8,7,2}; // 7
        faces[7] = {8,2,1};  // 8
        faces[8] = {2,7,5};  // 8
        faces[9] = {2,5,3};  // 8
        faces[10] = {6,8,1};  // 8
        faces[11] = {6,1,4};  // 8
    }

    else if(meshEnum == Triangle){
        faces = std::make_unique<Face[]>(1);
        vertices = std::make_unique<Vector3[]>(3);
        _num_triangles = 1;

        vertices[0] = {-1, -1, -1};
        vertices[1] = {-1, 1,  -1};
        vertices[2] = {1,  1,  -1};
        faces[0] = {1,2,3};
    }
}

