#include "geometry.h"


Triangle::Triangle(Vector2 v1, Vector2 v2, Vector3 v3){

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
        faces = std::make_unique<Face[]>(8);
        vertices = std::make_unique<Vector3[]>(12);
        _num_triangles = 8;

    }
}
