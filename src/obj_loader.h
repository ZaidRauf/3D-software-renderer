#pragma once

#include <array>
#include <fstream>
#include "geometry.h"
#include "linalg.h"
#include <vector>
#include <string>
#include <iostream>

class OBJLoader{
    struct IndexStruct {
        int a = -1;
        int b = -1;
        int c = -1;
        
        IndexStruct(int a, int b, int c) : a(a), b(b), c(c) {};
        ~IndexStruct(){};
    };
    using IndexStruct = struct IndexStruct;

    public:
        OBJLoader();
        OBJLoader(const std::string &filename);
        ~OBJLoader();
        std::vector<Vector3> vertices;
        std::vector<Vector2> uvs;
        std::vector<IndexStruct> face_indices;
        std::vector<IndexStruct> uv_indices;
        int vertex_count = 0;
        int uv_count = 0;
        int face_count = 0;
        int face_idx_count = 0;
        int uv_idx_count = 0;
};