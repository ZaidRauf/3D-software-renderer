#include "linalg.h"

// Vector2 Function Implementations
Vector2::Vector2() : x(0), y(0){}

Vector2::Vector2(float x, float y) : x(x), y(y){}

Vector2::Vector2(const Vector2 &vector) : x(vector.x), y(vector.y){}

Vector2::Vector2(const Vector3 &vector) : x(vector.x), y(vector.y){}

Vector2::Vector2(const Vector4 &vector) : x(vector.x), y(vector.y){}

Vector2::~Vector2(){}

float Vector2::Length(){
    return sqrt(Vector2::Dot(*this, *this));
}

Vector2 Vector2::Normalized(){
    return *this * (1/this->Length());
}

Vector2 Vector2::Add(const Vector2& a, const Vector2& b){
    return Vector2(a.x + b.x, a.y + b.y);
}

Vector2 Vector2::Subtract(const Vector2 &a, const Vector2 &b){
    return Vector2(a.x - b.x, a.y - b.y);
}

float Vector2::Dot(const Vector2 &a, const Vector2 &b){
    return (a.x * b.x) + (a.y * b.y);
}

Vector2 Vector2::ScalarMultiply(const float &scalar, const Vector2 &vector){
    return Vector2(scalar * vector.x, scalar * vector.y);
}

Vector2 Vector2::operator=(const Vector2 &v){
    this->x = v.x;
    this->y = v.y;
    return *this;
}

// Vector2 Operator Implementations
Vector2 operator+(const Vector2 &a, const Vector2 &b){
    return Vector2::Add(a, b);

}

Vector2 operator-(const Vector2 &a, const Vector2 &b){
    return Vector2::Subtract(a, b);
}

float operator*(const Vector2 &a, const Vector2 &b){
    return Vector2::Dot(a, b);
}

Vector2 operator*(const float &scalar, const Vector2 &vector){
    return Vector2::ScalarMultiply(scalar, vector);
}

Vector2 operator*(const Vector2 &vector, const float &scalar){
    return Vector2::ScalarMultiply(scalar, vector);
}

std::ostream& operator<<(std::ostream &os, const Vector2 &v){
    return os << "Vector2: (x: " << v.x << ", y: " << v.y << ")";
}

// Vector3 Function Implementations
Vector3::Vector3() : x(0), y(0), z(0){}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z){}

Vector3::Vector3(const Vector2 &vector) : x(vector.x), y(vector.y), z(0){}

Vector3::Vector3(const Vector3 &vector) : x(vector.x), y(vector.y), z(vector.z){}

Vector3::Vector3(const Vector4 &vector) : x(vector.x), y(vector.y), z(vector.z){}

Vector3::~Vector3(){}

float Vector3::Length(){
    return sqrt(Vector3::Dot(*this, *this));
}

Vector3 Vector3::Normalized(){
    return *this * (1/this->Length());
}

Vector3 Vector3::Add(const Vector3& a, const Vector3& b){
    return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3 Vector3::Subtract(const Vector3 &a, const Vector3 &b){
    return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

float Vector3::Dot(const Vector3 &a, const Vector3 &b){
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Vector3 Vector3::ScalarMultiply(const float &scalar, const Vector3 &vector){
    return Vector3(scalar * vector.x, scalar * vector.y, scalar * vector.z);
}

Vector3 Vector3::operator=(const Vector3 &v){
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    return *this;
}

Vector3 Vector3::Cross(const Vector3 &a, const Vector3 &b){
    return Vector3(
            (a.y * b.z) - (a.z * b.y),
            (a.z * b.x) - (a.x * b.z),
            (a.y * b.z) - (a.z * b.y)
            );
}

// Vector3 Operator Implementations
Vector3 operator+(const Vector3 &a, const Vector3 &b){
    return Vector3::Add(a, b);

}

Vector3 operator-(const Vector3 &a, const Vector3 &b){
    return Vector3::Subtract(a, b);
}

float operator*(const Vector3 &a, const Vector3 &b){
    return Vector3::Dot(a, b);
}

Vector3 operator*(const float &scalar, const Vector3 &vector){
    return Vector3::ScalarMultiply(scalar, vector);
}

Vector3 operator*(const Vector3 &vector, const float &scalar){
    return Vector3::ScalarMultiply(scalar, vector);
}

std::ostream& operator<<(std::ostream &os, const Vector3 &v){
    return os << "Vector3: (x: " << v.x << ", y: " << v.y << ", z: " << v.z << ")";
}

// Vector4 Function Implementations
Vector4::Vector4() : x(0), y(0), z(0), w(0){}

Vector4::Vector4(float x, float y, float z, float w): x(x), y(y), z(z), w(w){}

Vector4::Vector4(const Vector2 &vector) : x(vector.x), y(vector.y), z(0), w(1){}

Vector4::Vector4(const Vector3 &vector) : x(vector.x), y(vector.y), z(vector.z), w(1){}

Vector4::Vector4(const Vector4 &vector) : x(vector.x), y(vector.y), z(vector.z), w(vector.w){}

Vector4::Vector4(const std::array<float, 4> &arr) : x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3]){}

Vector4::~Vector4(){}

float Vector4::Length(){
    return sqrt(Vector4::Dot(*this, *this));
}

Vector4 Vector4::Normalized(){
    return *this * (1/this->Length());
}

Vector4 Vector4::Add(const Vector4& a, const Vector4& b){
    return Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Vector4 Vector4::Subtract(const Vector4 &a, const Vector4 &b){
    return Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.z - b.z);
}

float Vector4::Dot(const Vector4 &a, const Vector4 &b){
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

Vector4 Vector4::ScalarMultiply(const float &scalar, const Vector4 &vector){
    return Vector4(scalar * vector.x, scalar * vector.y, scalar * vector.z, scalar * vector.w);
}

Vector4 Vector4::operator=(const Vector4 &v){
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    this->w = v.w;
    return *this;
}

// Vector4 Operator Implementations
Vector4 operator+(const Vector4 &a, const Vector4 &b){
    return Vector4::Add(a, b);
}

Vector4 operator-(const Vector4 &a, const Vector4 &b){
    return Vector4::Subtract(a, b);
}

float operator*(const Vector4 &a, const Vector4 &b){
    return Vector4::Dot(a, b);
}

Vector4 operator*(const float &scalar, const Vector4 &vector){
    return Vector4::ScalarMultiply(scalar, vector);
}

Vector4 operator*(const Vector4 &vector, const float &scalar){
    return Vector4::ScalarMultiply(scalar, vector);
}

std::ostream& operator<<(std::ostream &os, const Vector4 &v){
    return os << "Vector4: (x: " << v.x << ", y: " << v.y << ", z: " << v.z << ", w: " << v.w << ")";
}

// TODO: Matrix 4x4 Implementation

Matrix4x4::Matrix4x4(){
    matrix = {{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    }};
}

Matrix4x4::~Matrix4x4(){}

const Vector4 Matrix4x4::MatrixVectorMultiply(const Vector4 &v){
    return Vector4(
            Vector4::Dot(Vector4(matrix[0]), v),
            Vector4::Dot(Vector4(matrix[1]), v),
            Vector4::Dot(Vector4(matrix[2]), v),
            Vector4::Dot(Vector4(matrix[3]), v)
            );   
}

Matrix4x4 Matrix4x4::Identity(){
   Matrix4x4 mat;

   mat.matrix = {{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    }}; 

   return mat; 
}

Matrix4x4 Matrix4x4::Zeros(){
   Matrix4x4 mat;

   mat.matrix = {{
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }}; 

   return mat; 
}

Matrix4x4 Matrix4x4::Translation(float tx, float ty, float tz){
   Matrix4x4 mat;

   mat.matrix = {{
        {1, 0, 0, tx},
        {0, 1, 0, ty},
        {0, 0, 1, tz},
        {0, 0, 0, 1}
    }}; 

   return mat; 
}


Matrix4x4 Matrix4x4::Scale(float sx, float sy, float sz){
   Matrix4x4 mat;

   mat.matrix = {{
        {sx, 0, 0, 0},
        {0, sy, 0, 0},
        {0, 0, sz, 0},
        {0, 0, 0, 1}
    }}; 

   return mat; 
}

// Working with floats hence using cosf, sinf
// All angles assumed to be passed as radians
Matrix4x4 Matrix4x4::ZRotationMatrix(float theta){
   Matrix4x4 mat;

   mat.matrix = {{
        {cosf(theta), -sinf(theta), 0, 0},
        {sinf(theta), cosf(theta),  0, 0},
        {0,           0,            1, 0},
        {0,           0,            0, 1}
    }}; 

   return mat; 
}

// Derive again later
Matrix4x4 Matrix4x4::YRotationMatrix(float theta){
   Matrix4x4 mat;

   mat.matrix = {{
        { cosf(theta),  0,  sinf(theta),  0},
        { 0,            1,  0,            0},
        {-sinf(theta),  0,  cosf(theta),  0},
        { 0,            0,  0,            1}
    }}; 
   
   return mat; 
}

Matrix4x4 Matrix4x4::XRotationMatrix(float theta){
   Matrix4x4 mat;

   mat.matrix = {{
        {1, 0,             0,              0},
        {0, cosf(theta),  -sinf(theta),    0},
        {0, sinf(theta),   cosf(theta),    0},
        {0, 0,             0,              1}
   }}; 

   return mat; 
}

const Vector4 Matrix4x4::operator*(const Vector4 &vector){
    return MatrixVectorMultiply(vector);
}


const Matrix4x4 Matrix4x4::MatrixMultiply(const Matrix4x4 &m){
    Matrix4x4 final_matrix = Matrix4x4::Zeros();

    for(auto m_row = 0; m_row < 4; m_row++){
        for(auto n_col = 0; n_col < 4; n_col++){
            final_matrix.matrix[m_row][n_col] = Vector4::Dot(Vector4(this->matrix[m_row]), Vector4(m.matrix[0][n_col], m.matrix[1][n_col], m.matrix[2][n_col], m.matrix[3][n_col]));
        }
    }

    return final_matrix;
}

const Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &m){
    return MatrixMultiply(m);
}

// FOV passed in radians
Matrix4x4 Matrix4x4::PerspectiveProjectionMatrix(float fov, float aspect_ratio, float z_near, float z_far){
    Matrix4x4 persp_matrix = Matrix4x4::Zeros();

    persp_matrix.matrix[0][0] = 1/(aspect_ratio * tan(fov/2));
    persp_matrix.matrix[1][1] = 1/(tan(fov/2));
    persp_matrix.matrix[2][2] = z_far / (z_far - z_near);
    persp_matrix.matrix[2][3] = - (z_far * z_near) / (z_far - z_near);
    persp_matrix.matrix[3][2] = 1.0;


    //persp_matrix.matrix[2][2] = 0;
    //persp_matrix.matrix[2][3] = 0;

    return persp_matrix;
}

// Extra Utility Functions
// Determinant of a 2x2 Matrix
// | a b |
// | c d | = ad - bc
inline float Determinant(const Vector2 &a, Vector2 const &b){
    return (a.x * b.y) - (a.y * b.x);
}
