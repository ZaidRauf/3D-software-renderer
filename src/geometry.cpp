#include "geometry.h"


Triangle::Triangle(Vector4 v1, Vector4 v2, Vector4 v3) : a(v1), b(v2), c(v3){
    uv_a = Vector2();
    uv_b = Vector2();
    uv_c = Vector2();
    flat_shading_intensity = 1.0;
}

Triangle::Triangle(Vector4 v1, Vector4 v2, Vector4 v3, Vector2 uv_a, Vector2 uv_b, Vector2 uv_c) : a(v1), b(v2), c(v3), uv_a(uv_a), uv_b(uv_b), uv_c(uv_c) {
    flat_shading_intensity = 1.0;
}

Triangle::~Triangle(){}

void Triangle::MapVerts(const Matrix4x4 &m){
    a = m * a;
    b = m * b;
    c = m * c;
}

Face::Face(){}

Face::Face(int a, int b, int c) : a(a), b(b), c(c){
    uv_a = 0;
    uv_b = 0;
    uv_c = 0;
};

Face::Face(int a, int b, int c, int uv_a, int uv_b, int uv_c) : a(a), b(b), c(c), uv_a(uv_a), uv_b(uv_b), uv_c(uv_c){};

Face::~Face(){};

Transform::Transform(){};
Transform::~Transform(){};

Mesh::Mesh(){};
Mesh::~Mesh(){};

Mesh::Mesh(DefaultMesh meshEnum){
    if(meshEnum == Cube){
        faces = std::make_unique<Face[]>(12);
        vertices = std::make_unique<Vector3[]>(8);
        uv_coords = std::make_unique<Vector2[]>(4);
        _num_triangles = 12;
        _num_vertices = 8;


        vertices[0] = {-1, -1, -1}; // 1
        vertices[1] = {-1, 1,  -1}; // 2
        vertices[2] = {1,  1,  -1}; // 3
        vertices[3] = {1,  -1, -1}; // 4
        vertices[4] = {1,  1,  1}; // 5
        vertices[5] = {1,  -1, 1}; // 6
        vertices[6] = {-1, 1,  1}; // 7
        vertices[7] = {-1, -1, 1};  // 8

        faces[0] = {1,2,3, 1, 0, 2}; // 1
        faces[1] = {1,3,4, 1, 2, 3}; // 2
        faces[2] = {4,3,5, 1, 0, 2}; // 3
        faces[3] = {4,5,6, 1, 2, 3}; // 4
        faces[4] = {6,5,7, 1, 0, 2}; // 5
        faces[5] = {6,7,8, 1, 2, 3}; // 6
        faces[6] = {8,7,2, 1, 0, 2}; // 7
        faces[7] = {8,2,1, 1, 2, 3};  // 8
        faces[8] = {2,7,5, 1, 0, 2};  // 8
        faces[9] = {2,5,3, 1, 2, 3};  // 8
        faces[10] = {6,8,1, 1, 0, 2};  // 8
        faces[11] = {6,1,4, 1, 2, 3};  // 8

        uv_coords[0] = {0, 0};
        uv_coords[1] = {0, 1};
        uv_coords[2] = {1, 0};
        uv_coords[2] = {1, 1};
    }

    else if(meshEnum == Triangle){
        uv_coords = std::make_unique<Vector2[]>(3); // set to 1 for default color
        // uv_coords[0] = {0, 0};

        faces = std::make_unique<Face[]>(1);
        vertices = std::make_unique<Vector3[]>(3);
        _num_triangles = 1;
        _num_vertices = 3;

        vertices[0] = {-1, -1, -1};
        vertices[1] = {-1, 1,  -1};
        vertices[2] = {1,  1,  -1};
        faces[0] = {1, 2, 3, 1, 0, 2};

        uv_coords[0] = {0, 0};
        uv_coords[1] = {0, 1};
        uv_coords[2] = {1, 0};
    }

    else if(meshEnum == Bunny){
        uv_coords = std::make_unique<Vector2[]>(1);
        uv_coords[0] = {0, 0};

        _num_triangles = 598;
        _num_vertices = 301;

        faces = std::unique_ptr<Face[]>( new Face[598]{
            {3, 2, 4},
            {3, 4, 1},
            {3, 6, 2},
            {1, 7, 3},
            {8, 7, 1},
            {10, 9, 5},
            {12, 1, 4},
            {9, 14, 23},
            {13, 9, 20},
            {5, 9, 13},
            {15, 16, 11},
            {12, 4, 26},
            {6, 3, 7},
            {1, 12, 8},
            {9, 23, 20},
            {14, 9, 10},
            {30, 14, 10},
            {24, 25, 21},
            {2, 41, 4},
            {19, 7, 8},
            {23, 33, 27},
            {13, 20, 28},
            {14, 33, 23},
            {57, 15, 11},
            {17, 35, 11},
            {16, 17, 11},
            {2, 22, 41},
            {41, 18, 4},
            {18, 26, 4},
            {23, 40, 20},
            {14, 29, 33},
            {57, 29, 15},
            {14, 30, 34},
            {34, 15, 29},
            {16, 24, 17},
            {31, 55, 39},
            {32, 22, 2},
            {27, 40, 23},
            {42, 56, 33},
            {34, 29, 14},
            {11, 35, 57},
            {5, 36, 10},
            {24, 21, 17},
            {39, 55, 37},
            {55, 31, 49},
            {6, 7, 19},
            {27, 33, 56},
            {27, 66, 40},
            {29, 42, 33},
            {43, 36, 5},
            {34, 16, 15},
            {45, 37, 55},
            {37, 47, 39},
            {21, 25, 38},
            {17, 91, 35},
            {32, 31, 39},
            {46, 32, 2},
            {46, 2, 6},
            {49, 31, 46},
            {67, 20, 89},
            {89, 20, 40},
            {28, 20, 67},
            {57, 42, 29},
            {13, 43, 5},
            {44, 36, 43},
            {44, 43, 59},
            {52, 44, 45},
            {47, 45, 44},
            {45, 47, 37},
            {24, 16, 53},
            {46, 31, 32},
            {6, 19, 71},
            {89, 40, 66},
            {50, 13, 28},
            {35, 58, 57},
            {43, 13, 59},
            {91, 51, 35},
            {10, 36, 30},
            {52, 36, 44},
            {60, 52, 54},
            {52, 45, 54},
            {55, 54, 45},
            {39, 47, 61},
            {53, 48, 24},
            {32, 39, 61},
            {55, 62, 54},
            {55, 49, 62},
            {89, 66, 85},
            {13, 50, 64},
            {44, 59, 47},
            {53, 16, 60},
            {38, 75, 21},
            {61, 68, 32},
            {68, 22, 32},
            {25, 24, 48},
            {46, 6, 71},
            {78, 19, 8},
            {27, 56, 66},
            {57, 79, 42},
            {57, 58, 79},
            {13, 64, 82},
            {60, 16, 52},
            {47, 59, 61},
            {18, 65, 26},
            {63, 62, 49},
            {56, 42, 73},
            {79, 73, 42},
            {58, 81, 79},
            {13, 50, 82},
            {58, 35, 51},
            {30, 16, 34},
            {52, 30, 36},
            {16, 30, 52},
            {75, 17, 21},
            {70, 26, 65},
            {49, 72, 63},
            {49, 46, 72},
            {82, 74, 59},
            {59, 13, 82},
            {94, 61, 59},
            {53, 60, 92},
            {76, 41, 22},
            {77, 48, 53},
            {41, 65, 18},
            {69, 38, 25},
            {71, 72, 46},
            {26, 115, 12},
            {88, 78, 8},
            {67, 80, 28},
            {50, 28, 80},
            {51, 83, 58},
            {77, 53, 92},
            {25, 48, 69},
            {78, 71, 19},
            {8, 84, 88},
            {93, 66, 56},
            {79, 90, 73},
            {82, 50, 80},
            {94, 59, 74},
            {17, 86, 91},
            {17, 75, 86},
            {38, 95, 75},
            {54, 92, 60},
            {68, 76, 22},
            {112, 72, 71},
            {26, 70, 115},
            {84, 8, 12},
            {102, 74, 82},
            {67, 101, 80},
            {68, 61, 94},
            {63, 87, 62},
            {56, 73, 93},
            {102, 98, 74},
            {96, 41, 76},
            {97, 70, 65},
            {115, 84, 12},
            {101, 67, 89},
            {102, 82, 80},
            {51, 91, 83},
            {99, 68, 94},
            {76, 68, 99},
            {41, 96, 65},
            {69, 100, 38},
            {116, 89, 85},
            {85, 66, 93},
            {73, 90, 93},
            {83, 103, 58},
            {121, 77, 92},
            {48, 121, 69},
            {58, 103, 81},
            {104, 121, 92},
            {38, 100, 95},
            {77, 121, 48},
            {69, 121, 100},
            {62, 105, 54},
            {106, 72, 112},
            {71, 78, 112},
            {78, 88, 112},
            {126, 89, 116},
            {85, 93, 116},
            {126, 101, 89},
            {102, 80, 114},
            {108, 74, 98},
            {109, 99, 94},
            {92, 105, 104},
            {54, 105, 92},
            {87, 105, 62},
            {110, 70, 97},
            {107, 84, 115},
            {93, 90, 125},
            {109, 94, 108},
            {86, 75, 95},
            {115, 70, 110},
            {87, 63, 127},
            {111, 63, 72},
            {113, 88, 84},
            {101, 114, 80},
            {81, 117, 79},
            {108, 94, 74},
            {96, 118, 65},
            {106, 111, 72},
            {119, 112, 88},
            {116, 93, 125},
            {140, 125, 90},
            {79, 117, 90},
            {81, 120, 117},
            {97, 65, 118},
            {122, 110, 97},
            {84, 123, 113},
            {124, 129, 125},
            {117, 140, 90},
            {120, 81, 103},
            {113, 119, 88},
            {98, 102, 128},
            {86, 151, 91},
            {84, 107, 123},
            {140, 124, 125},
            {91, 151, 83},
            {109, 130, 99},
            {76, 99, 130},
            {130, 96, 76},
            {132, 100, 121},
            {127, 105, 87},
            {125, 129, 139},
            {141, 116, 125},
            {142, 102, 114},
            {151, 103, 83},
            {105, 149, 104},
            {173, 129, 124},
            {128, 102, 142},
            {130, 109, 148},
            {131, 121, 104},
            {131, 132, 121},
            {111, 127, 63},
            {141, 125, 139},
            {154, 126, 116},
            {101, 134, 114},
            {135, 98, 128},
            {108, 98, 147},
            {95, 136, 86},
            {130, 118, 96},
            {143, 133, 100},
            {132, 143, 100},
            {138, 106, 112},
            {173, 139, 129},
            {116, 141, 154},
            {134, 101, 126},
            {147, 109, 108},
            {122, 97, 118},
            {100, 133, 95},
            {110, 107, 115},
            {111, 106, 138},
            {126, 150, 134},
            {120, 145, 117},
            {146, 142, 114},
            {137, 136, 95},
            {130, 157, 118},
            {131, 104, 149},
            {137, 95, 133},
            {112, 119, 144},
            {139, 174, 141},
            {126, 154, 150},
            {117, 153, 140},
            {105, 127, 156},
            {152, 127, 111},
            {138, 152, 111},
            {124, 140, 153},
            {114, 134, 146},
            {147, 98, 135},
            {122, 118, 158},
            {155, 110, 122},
            {165, 120, 103},
            {151, 86, 166},
            {178, 158, 118},
            {113, 144, 119},
            {142, 135, 128},
            {178, 118, 160},
            {118, 157, 160},
            {161, 131, 149},
            {105, 162, 149},
            {132, 159, 143},
            {174, 139, 173},
            {153, 173, 124},
            {164, 154, 141},
            {109, 147, 170},
            {184, 86, 136},
            {171, 130, 148},
            {167, 158, 178},
            {161, 132, 131},
            {168, 107, 110},
            {152, 172, 127},
            {145, 169, 117},
            {151, 166, 103},
            {86, 184, 166},
            {170, 148, 109},
            {130, 171, 157},
            {156, 162, 105},
            {155, 168, 110},
            {127, 172, 156},
            {168, 123, 107},
            {112, 144, 163},
            {176, 150, 154},
            {165, 145, 120},
            {182, 138, 112},
            {112, 163, 182},
            {174, 164, 141},
            {175, 164, 174},
            {165, 103, 210},
            {177, 135, 142},
            {177, 147, 135},
            {166, 187, 103},
            {137, 179, 136},
            {173, 153, 205},
            {150, 183, 134},
            {136, 179, 184},
            {178, 189, 167},
            {180, 168, 155},
            {204, 144, 113},
            {183, 146, 134},
            {103, 187, 210},
            {137, 133, 179},
            {162, 161, 149},
            {132, 161, 159},
            {143, 185, 133},
            {180, 181, 168},
            {123, 168, 191},
            {138, 182, 152},
            {123, 204, 113},
            {146, 222, 142},
            {222, 177, 142},
            {216, 122, 158},
            {157, 155, 188},
            {188, 155, 122},
            {185, 143, 159},
            {173, 192, 174},
            {186, 205, 153},
            {164, 176, 154},
            {117, 198, 153},
            {169, 198, 117},
            {189, 178, 193},
            {171, 148, 190},
            {191, 204, 123},
            {174, 192, 175},
            {216, 158, 167},
            {188, 160, 157},
            {157, 203, 155},
            {172, 196, 156},
            {163, 144, 204},
            {181, 191, 168},
            {172, 152, 182},
            {169, 145, 199},
            {178, 160, 193},
            {148, 170, 194},
            {190, 148, 194},
            {122, 216, 195},
            {171, 203, 157},
            {203, 200, 155},
            {200, 180, 155},
            {207, 156, 196},
            {153, 198, 186},
            {165, 199, 145},
            {184, 187, 166},
            {193, 160, 202},
            {133, 185, 179},
            {161, 218, 211},
            {218, 161, 162},
            {195, 188, 122},
            {203, 171, 190},
            {161, 185, 159},
            {162, 156, 207},
            {204, 191, 203},
            {173, 205, 192},
            {164, 206, 176},
            {150, 176, 197},
            {199, 226, 169},
            {210, 199, 165},
            {170, 147, 201},
            {179, 217, 184},
            {163, 204, 190},
            {190, 204, 203},
            {182, 163, 212},
            {181, 208, 191},
            {146, 183, 222},
            {188, 202, 160},
            {202, 188, 195},
            {172, 182, 196},
            {198, 169, 226},
            {184, 217, 187},
            {211, 185, 161},
            {212, 190, 194},
            {212, 163, 190},
            {209, 208, 181},
            {213, 205, 186},
            {230, 164, 175},
            {197, 183, 150},
            {147, 228, 201},
            {189, 193, 215},
            {193, 214, 215},
            {217, 219, 187},
            {189, 215, 167},
            {229, 194, 170},
            {218, 162, 207},
            {203, 191, 221},
            {164, 230, 206},
            {206, 197, 176},
            {198, 213, 186},
            {238, 177, 222},
            {187, 233, 210},
            {177, 228, 147},
            {229, 170, 239},
            {211, 218, 220},
            {203, 221, 200},
            {237, 182, 212},
            {222, 183, 225},
            {201, 239, 170},
            {196, 182, 237},
            {192, 224, 175},
            {175, 224, 230},
            {197, 225, 183},
            {198, 226, 232},
            {223, 193, 202},
            {275, 216, 167},
            {216, 262, 195},
            {227, 220, 218},
            {177, 231, 228},
            {187, 219, 233},
            {220, 240, 211},
            {212, 235, 237},
            {235, 212, 194},
            {250, 207, 196},
            {205, 224, 192},
            {232, 213, 198},
            {210, 233, 199},
            {231, 177, 238},
            {193, 223, 214},
            {275, 167, 215},
            {262, 216, 275},
            {195, 223, 202},
            {235, 194, 229},
            {250, 218, 207},
            {250, 196, 237},
            {191, 208, 221},
            {199, 233, 226},
            {223, 195, 262},
            {217, 179, 234},
            {213, 224, 205},
            {213, 230, 224},
            {213, 232, 230},
            {230, 247, 206},
            {197, 253, 225},
            {226, 265, 232},
            {233, 242, 226},
            {239, 201, 228},
            {243, 262, 275},
            {185, 236, 179},
            {244, 185, 211},
            {218, 250, 227},
            {230, 232, 247},
            {226, 242, 265},
            {222, 225, 238},
            {254, 219, 217},
            {260, 240, 220},
            {248, 229, 239},
            {244, 211, 240},
            {248, 235, 229},
            {185, 244, 236},
            {227, 250, 245},
            {251, 237, 235},
            {246, 180, 200},
            {219, 254, 233},
            {223, 258, 214},
            {234, 254, 217},
            {249, 235, 248},
            {249, 251, 235},
            {250, 237, 251},
            {181, 180, 252},
            {227, 260, 220},
            {245, 250, 251},
            {180, 246, 252},
            {255, 209, 181},
            {206, 256, 197},
            {256, 206, 259},
            {259, 206, 247},
            {232, 241, 247},
            {238, 225, 264},
            {233, 257, 242},
            {254, 257, 233},
            {252, 255, 181},
            {197, 256, 253},
            {265, 241, 232},
            {264, 225, 268},
            {231, 238, 264},
            {258, 223, 262},
            {221, 246, 200},
            {261, 267, 209},
            {208, 209, 267},
            {256, 268, 253},
            {253, 268, 225},
            {228, 231, 270},
            {228, 270, 239},
            {263, 260, 227},
            {179, 236, 234},
            {266, 245, 251},
            {266, 251, 249},
            {271, 239, 270},
            {214, 272, 215},
            {254, 234, 257},
            {271, 248, 239},
            {240, 276, 244},
            {273, 249, 248},
            {273, 266, 249},
            {266, 263, 245},
            {242, 257, 265},
            {270, 231, 264},
            {275, 215, 272},
            {273, 248, 271},
            {263, 227, 245},
            {264, 268, 269},
            {269, 270, 264},
            {271, 270, 269},
            {240, 283, 276},
            {240, 260, 283},
            {221, 287, 246},
            {208, 267, 221},
            {265, 257, 274},
            {274, 257, 276},
            {243, 258, 262},
            {234, 244, 276},
            {266, 273, 277},
            {267, 287, 221},
            {259, 286, 256},
            {256, 286, 268},
            {278, 259, 247},
            {268, 284, 269},
            {274, 279, 265},
            {269, 284, 280},
            {269, 280, 271},
            {214, 258, 272},
            {276, 257, 234},
            {236, 244, 234},
            {287, 281, 246},
            {252, 246, 288},
            {261, 209, 255},
            {282, 247, 241},
            {241, 265, 279},
            {286, 259, 278},
            {284, 268, 286},
            {247, 282, 278},
            {294, 260, 263},
            {252, 285, 255},
            {279, 282, 241},
            {275, 272, 243},
            {272, 258, 243},
            {294, 263, 266},
            {285, 252, 288},
            {286, 280, 284},
            {276, 283, 274},
            {277, 273, 271},
            {296, 266, 277},
            {297, 267, 261},
            {282, 279, 289},
            {283, 291, 274},
            {277, 271, 280},
            {294, 283, 260},
            {296, 294, 266},
            {278, 290, 286},
            {274, 291, 279},
            {289, 279, 291},
            {282, 289, 278},
            {278, 289, 290},
            {286, 290, 280},
            {290, 293, 280},
            {280, 293, 277},
            {281, 288, 246},
            {289, 295, 290},
            {291, 295, 289},
            {298, 277, 293},
            {291, 283, 295},
            {292, 297, 261},
            {290, 295, 293},
            {283, 294, 295},
            {298, 295, 294},
            {298, 296, 277},
            {294, 296, 298},
            {295, 298, 293},
            {255, 292, 261},
            {299, 285, 288},
            {285, 292, 255},
            {287, 267, 300},
            {300, 281, 287},
            {301, 288, 281},
            {267, 297, 300},
            {288, 301, 299},
            {300, 301, 281},
            {285, 299, 292},
            {297, 301, 300},
            {301, 297, 292},
            {299, 301, 292}
        });

        vertices = std::unique_ptr<Vector3[]>( new Vector3[301]{
            {0.281389 ,0.237772 ,0.396183},
            {0.291839 ,0.161583 ,0.294042},
            {0.272923 ,0.177726 ,0.370747},
            {0.284962 ,0.271350 ,0.349622},
            {0.332578 ,-0.024776 ,0.014895},
            {0.220594 ,0.115436 ,0.371521},
            {0.249410 ,0.152993 ,0.417784},
            {0.199268 ,0.227953 ,0.450117},
            {0.357953 ,-0.128148 ,-0.086260},
            {0.326486 ,-0.110353 ,0.021204},
            {0.326021 ,-0.363478 ,0.060167},
            {0.188364 ,0.331038 ,0.417003},
            {0.278688 ,0.037606 ,-0.087797},
            {0.326849 ,-0.222235 ,-0.035139},
            {0.312366 ,-0.298546 ,0.035356},
            {0.231787 ,-0.306959 ,0.131939},
            {0.237230 ,-0.373944 ,0.139623},
            {0.221870 ,0.335746 ,0.278460},
            {0.188183 ,0.150338 ,0.450251},
            {0.350103 ,-0.050338 ,-0.176803},
            {0.250201 ,-0.371581 ,0.246398},
            {0.226472 ,0.200378 ,0.241036},
            {0.348041 ,-0.191262 ,-0.132025},
            {0.259108 ,-0.317968 ,0.259340},
            {0.203398 ,-0.354471 ,0.312669},
            {0.213123 ,0.371852 ,0.325333},
            {0.326935 ,-0.206016 ,-0.199429},
            {0.314545 ,0.014566 ,-0.174109},
            {0.291301 ,-0.306929 ,-0.047145},
            {0.208088 ,-0.226681 ,0.049928},
            {0.226663 ,0.026281 ,0.305232},
            {0.191136 ,0.126177 ,0.271955},
            {0.333412 ,-0.269090 ,-0.124702},
            {0.271817 ,-0.274216 ,0.014975},
            {0.265107 ,-0.381167 ,0.021895},
            {0.237215 ,-0.091189 ,0.070319},
            {0.254239 ,-0.038479 ,0.193583},
            {0.175161 ,-0.372160 ,0.261660},
            {0.239125 ,0.043840 ,0.241387},
            {0.335344 ,-0.118557 ,-0.222962},
            {0.183989 ,0.287634 ,0.245014},
            {0.281760 ,-0.334364 ,-0.161025},
            {0.258749 ,0.008309 ,0.039078},
            {0.248345 ,-0.015160 ,0.115602},
            {0.236769 ,-0.114697 ,0.168334},
            {0.180246 ,0.091801 ,0.345521},
            {0.238074 ,0.035682 ,0.167941},
            {0.144922 ,-0.302949 ,0.285651},
            {0.194505 ,-0.035648 ,0.363593},
            {0.274667 ,0.066565 ,-0.153813},
            {0.209615 ,-0.350451 ,0.048018},
            {0.234513 ,-0.168340 ,0.115921},
            {0.201265 ,-0.266745 ,0.190493},
            {0.179863 ,-0.181803 ,0.207972},
            {0.230810 ,-0.091563 ,0.274087},
            {0.303267 ,-0.271352 ,-0.226493},
            {0.288695 ,-0.381684 ,-0.081772},
            {0.226013 ,-0.365074 ,-0.075476},
            {0.236715 ,0.085751 ,0.032494},
            {0.214939 ,-0.222588 ,0.166601},
            {0.210506 ,0.108177 ,0.146010},
            {0.163620 ,-0.143979 ,0.321617},
            {0.094009 ,-0.063724 ,0.412254},
            {0.269328 ,0.056136 ,-0.105789},
            {0.162340 ,0.390034 ,0.212963},
            {0.283235 ,-0.175470 ,-0.284283},
            {0.289982 ,0.030098 ,-0.256440},
            {0.173726 ,0.149998 ,0.183474},
            {0.135066 ,-0.358289 ,0.308535},
            {0.131413 ,0.408460 ,0.344995},
            {0.123959 ,0.118434 ,0.384653},
            {0.097470 ,0.021773 ,0.430581},
            {0.267829 ,-0.305447 ,-0.239286},
            {0.206469 ,0.161938 ,-0.033116},
            {0.143816 ,-0.380080 ,0.167551},
            {0.141816 ,0.248737 ,0.161154},
            {0.135334 ,-0.277178 ,0.221593},
            {0.127409 ,0.179710 ,0.448451},
            {0.261229 ,-0.389398 ,-0.220302},
            {0.239838 ,0.107653 ,-0.231895},
            {0.206798 ,-0.382689 ,-0.190557},
            {0.259524 ,0.109129 ,-0.128697},
            {0.167315 ,-0.358642 ,-0.024738},
            {0.046395 ,0.298450 ,0.425879},
            {0.275999 ,-0.113472 ,-0.326905},
            {0.094349 ,-0.384882 ,0.075301},
            {0.066524 ,-0.142619 ,0.362998},
            {0.084315 ,0.210263 ,0.464383},
            {0.288034 ,-0.031617 ,-0.313707},
            {0.233989 ,-0.332770 ,-0.290332},
            {0.178252 ,-0.362860 ,0.089492},
            {0.134839 ,-0.210903 ,0.209146},
            {0.248792 ,-0.229103 ,-0.346620},
            {0.170592 ,0.154571 ,0.061348},
            {0.080510 ,-0.379521 ,0.201913},
            {0.109465 ,0.304688 ,0.179952},
            {0.103420 ,0.409805 ,0.217570},
            {0.160030 ,0.214093 ,-0.083355},
            {0.115938 ,0.200618 ,0.136861},
            {0.065881 ,-0.364370 ,0.289187},
            {0.223052 ,0.084850 ,-0.330439},
            {0.199457 ,0.194348 ,-0.183261},
            {0.121433 ,-0.363841 ,-0.109019},
            {0.069364 ,-0.244994 ,0.187335},
            {0.039705 ,-0.206737 ,0.301748},
            {0.025987 ,0.063374 ,0.429191},
            {0.001571 ,0.380317 ,0.403685},
            {0.098343 ,0.195301 ,0.027236},
            {0.095072 ,0.213982 ,0.077500},
            {0.022369 ,0.416802 ,0.320916},
            {0.007447 ,-0.007363 ,0.428786},
            {0.012016 ,0.120203 ,0.414755},
            {-0.021463 ,0.272686 ,0.422552},
            {0.182278 ,0.170342 ,-0.286498},
            {0.066951 ,0.388842 ,0.428572},
            {0.207654 ,-0.109014 ,-0.394487},
            {0.131917 ,-0.391808 ,-0.294714},
            {0.048608 ,0.379168 ,0.167850},
            {-0.005179 ,0.180468 ,0.441404},
            {0.131787 ,-0.395480 ,-0.184135},
            {0.079306 ,-0.302115 ,0.207770},
            {-0.009128 ,0.410782 ,0.225398},
            {-0.036163 ,0.326852 ,0.377116},
            {0.199530 ,-0.323053 ,-0.409079},
            {0.212416 ,-0.268624 ,-0.360149},
            {0.209437 ,-0.000104 ,-0.386212},
            {-0.016746 ,-0.105538 ,0.390180},
            {0.129002 ,0.231900 ,-0.139594},
            {0.217420 ,-0.265086 ,-0.420687},
            {0.039673 ,0.253574 ,0.137200},
            {0.017894 ,-0.284477 ,0.233111},
            {-0.001976 ,-0.296648 ,0.296414},
            {-0.012908 ,-0.378759 ,0.269684},
            {0.147770 ,0.131149 ,-0.356776},
            {0.070335 ,0.233020 ,-0.089692},
            {0.002590 ,-0.378245 ,0.142157},
            {0.014202 ,-0.378756 ,0.210171},
            {-0.042690 ,0.056802 ,0.404644},
            {0.183920 ,-0.193985 ,-0.482295},
            {0.161712 ,-0.369273 ,-0.337411},
            {0.151281 ,-0.165601 ,-0.426473},
            {0.098578 ,0.230108 ,-0.227481},
            {-0.029080 ,-0.367442 ,0.341999},
            {-0.073480 ,0.191369 ,0.394010},
            {0.077522 ,-0.383953 ,-0.264486},
            {0.072368 ,0.190393 ,-0.305827},
            {0.026897 ,0.218225 ,-0.029063},
            {0.013610 ,0.229357 ,0.082288},
            {-0.020083 ,-0.220814 ,0.233937},
            {0.081643 ,0.066673 ,-0.408344},
            {0.069428 ,-0.358990 ,0.030190},
            {-0.078652 ,-0.017038 ,0.400053},
            {0.112206 ,-0.360411 ,-0.419634},
            {0.147859 ,-0.027520 ,-0.425832},
            {-0.051251 ,0.397827 ,0.240349},
            {-0.064920 ,-0.152750 ,0.321213},
            {-0.033980 ,0.314730 ,0.157996},
            {-0.010391 ,0.472086 ,0.129041},
            {-0.086133 ,-0.337553 ,0.299605},
            {-0.035262 ,0.344595 ,0.100894},
            {-0.075795 ,-0.289173 ,0.225190},
            {-0.073062 ,-0.204035 ,0.226156},
            {-0.094992 ,0.141557 ,0.325674},
            {0.080371 ,-0.145734 ,-0.442225},
            {0.021801 ,-0.394042 ,-0.154210},
            {-0.010187 ,-0.355872 ,0.038730},
            {-0.038779 ,0.516476 ,0.042158},
            {-0.065240 ,0.392183 ,0.335267},
            {0.048116 ,-0.401130 ,-0.301189},
            {-0.022962 ,0.199552 ,0.019460},
            {-0.037572 ,0.242034 ,0.145033},
            {-0.089911 ,-0.090932 ,0.364333},
            {0.151360 ,-0.277423 ,-0.506679},
            {0.140824 ,-0.161037 ,-0.506262},
            {0.049305 ,-0.158804 ,-0.495727},
            {0.067721 ,-0.031722 ,-0.440828},
            {-0.016694 ,0.228534 ,-0.149583},
            {0.006895 ,0.398533 ,0.099057},
            {-0.090615 ,-0.382939 ,0.141625},
            {-0.123845 ,0.454244 ,0.267341},
            {-0.122476 ,0.438634 ,0.312164},
            {-0.135048 ,0.029832 ,0.340509},
            {0.028382 ,0.118715 ,-0.377892},
            {-0.035059 ,-0.359224 ,0.083350},
            {-0.128200 ,-0.379746 ,0.176007},
            {0.042119 ,-0.352054 ,-0.433774},
            {-0.064605 ,-0.355723 ,0.007650},
            {-0.062299 ,0.379296 ,0.177910},
            {-0.048334 ,0.451553 ,0.053157},
            {-0.091896 ,0.211068 ,0.190935},
            {-0.100664 ,0.326661 ,0.302769},
            {0.049486 ,-0.211512 ,-0.527137},
            {-0.045685 ,0.414861 ,-0.034345},
            {-0.115078 ,0.198785 ,0.128484},
            {-0.107396 ,0.431919 ,0.102040},
            {-0.162024 ,-0.067046 ,0.297604},
            {-0.017944 ,-0.014312 ,-0.428961},
            {-0.004874 ,-0.371826 ,-0.360436},
            {-0.030171 ,-0.396120 ,-0.197048},
            {-0.132758 ,0.384536 ,0.218361},
            {-0.071181 ,0.186164 ,-0.057364},
            {-0.084873 ,0.381053 ,0.058529},
            {-0.098650 ,0.313467 ,0.241874},
            {-0.109816 ,0.236028 ,0.308629},
            {0.026805 ,-0.310807 ,-0.486391},
            {-0.018161 ,-0.114963 ,-0.430913},
            {-0.145565 ,-0.137129 ,0.245755},
            {-0.182883 ,0.371534 ,0.307894},
            {-0.172696 ,0.410301 ,0.313567},
            {-0.034492 ,-0.363727 ,-0.152381},
            {-0.111359 ,-0.297362 ,0.134353},
            {-0.136444 ,0.117960 ,0.253297},
            {-0.031601 ,-0.310229 ,-0.422215},
            {-0.101495 ,0.442468 ,-0.100375},
            {-0.111541 ,0.495402 ,-0.034620},
            {-0.087214 ,0.512784 ,0.089660},
            {-0.125299 ,-0.353808 ,0.082484},
            {-0.120920 ,-0.177113 ,0.167703},
            {-0.122617 ,-0.353909 ,0.010361},
            {-0.096919 ,-0.250454 ,0.065048},
            {-0.187972 ,0.341532 ,0.246173},
            {-0.011538 ,0.206081 ,-0.267442},
            {-0.122350 ,0.429896 ,-0.028539},
            {-0.018091 ,-0.230531 ,-0.471886},
            {-0.040537 ,0.104473 ,-0.366570},
            {-0.033816 ,-0.386871 ,-0.261051},
            {-0.149959 ,-0.135135 ,0.076749},
            {-0.083108 ,0.185409 ,-0.129343},
            {-0.134686 ,0.140527 ,0.061420},
            {-0.019788 ,-0.223354 ,-0.409652},
            {-0.116822 ,0.156891 ,-0.199789},
            {-0.046425 ,-0.311664 ,-0.338749},
            {-0.096403 ,-0.370573 ,-0.170027},
            {-0.210990 ,-0.388487 ,0.018341},
            {-0.161513 ,0.108979 ,0.150342},
            {-0.224744 ,-0.381389 ,0.120800},
            {-0.171274 ,0.029501 ,0.232515},
            {-0.083164 ,0.167566 ,-0.256739},
            {-0.135502 ,0.131083 ,-0.045763},
            {-0.172751 ,-0.281718 ,0.039939},
            {-0.126142 ,-0.300943 ,-0.318266},
            {-0.115819 ,-0.395727 ,-0.243825},
            {-0.183997 ,0.584418 ,-0.078222},
            {-0.212869 ,-0.317354 ,0.091691},
            {-0.165504 ,-0.083538 ,0.076753},
            {-0.234475 ,0.437747 ,0.202100},
            {-0.100112 ,-0.244073 ,-0.359569},
            {-0.172283 ,0.075198 ,0.034791},
            {-0.172152 ,0.054156 ,0.075678},
            {-0.181902 ,-0.072871 ,0.189471},
            {-0.183707 ,0.030858 ,0.150554},
            {-0.238790 ,0.507946 ,0.266636},
            {-0.081424 ,0.025444 ,-0.379846},
            {-0.154038 ,-0.385967 ,-0.016028},
            {-0.306777 ,0.500636 ,0.321057},
            {-0.102536 ,-0.071439 ,-0.398565},
            {-0.177644 ,-0.388380 ,-0.084366},
            {-0.186293 ,0.482477 ,-0.116216},
            {-0.114612 ,-0.146743 ,-0.367045},
            {-0.202515 ,-0.210942 ,0.028242},
            {-0.299472 ,0.451035 ,0.276289},
            {-0.146240 ,0.509251 ,0.007266},
            {-0.226570 ,-0.117391 ,0.047583},
            {-0.128831 ,0.087782 ,-0.255231},
            {-0.151969 ,-0.358105 ,-0.253027},
            {-0.203291 ,-0.046887 ,0.049972},
            {-0.332529 ,0.371358 ,0.270872},
            {-0.131864 ,0.026012 ,-0.320662},
            {-0.146103 ,0.045460 ,-0.213761},
            {-0.156091 ,0.100120 ,-0.141880},
            {-0.176983 ,0.039954 ,-0.093220},
            {-0.203122 ,0.542469 ,-0.132457},
            {-0.190537 ,0.017594 ,-0.010210},
            {-0.194693 ,-0.328768 ,-0.116083},
            {-0.133585 ,0.595717 ,-0.067512},
            {-0.208917 ,-0.306754 ,-0.013703},
            {-0.251481 ,-0.006813 ,-0.027818},
            {-0.186385 ,-0.187393 ,-0.287262},
            {-0.192564 ,-0.296530 ,-0.211263},
            {-0.208194 ,0.001332 ,-0.208951},
            {-0.321722 ,0.455343 ,0.202716},
            {-0.185035 ,-0.253513 ,-0.275525},
            {-0.254189 ,-0.230214 ,-0.048286},
            {-0.163303 ,-0.000540 ,-0.260883},
            {-0.371213 ,0.551273 ,0.306541},
            {-0.175247 ,-0.092547 ,-0.306382},
            {-0.325708 ,0.402658 ,0.220915},
            {-0.325501 ,0.534465 ,0.251293},
            {-0.214710 ,-0.196900 ,-0.208256},
            {-0.227802 ,-0.122422 ,-0.218516},
            {-0.244588 ,-0.205948 ,-0.135527},
            {-0.387652 ,0.501657 ,0.248827},
            {-0.240391 ,-0.040421 ,-0.176289},
            {-0.269943 ,-0.153204 ,-0.009722},
            {-0.258864 ,-0.133192 ,-0.120180},
            {-0.274734 ,-0.080983 ,-0.002237},
            {-0.387575 ,0.412672 ,0.251595},
            {-0.275565 ,-0.068935 ,-0.081711},
            {-0.413115 ,0.584094 ,0.238732},
            {-0.429230 ,0.458223 ,0.194523},
            {-0.447524 ,0.536915 ,0.187296}
        });

    }
}

