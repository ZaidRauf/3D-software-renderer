#include "geometry.h"


Triangle::Triangle(Vector2 v1, Vector2 v2, Vector3 v3){

}

Triangle::~Triangle(){}

Face::Face(){}

Face::Face(Vector3 v1, Vector3 v2, Vector3 v3) : a(v1), b(v2), c(v3){};

Face::~Face(){};

Transform::Transform(){};
Transform::~Transform(){};

Mesh::Mesh(){};
Mesh::~Mesh(){};


