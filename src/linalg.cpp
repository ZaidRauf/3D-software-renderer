#include "linalg.h"

// Vector2 Function Implementations
Vector2::Vector2() : x(0), y(0){}

Vector2::Vector2(float x, float y) : x(x), y(y){}

Vector2::Vector2(const Vector2 &vector) : x(vector.x), y(vector.y){}

Vector2::~Vector2(){

}

Vector2 Vector2::Add(const Vector2& a, const Vector2& b){
    return Vector2(a.x + b.x, a.y + b.y);
}

Vector2 Vector2::Subtract(const Vector2 &a, const Vector2 &b){
    return Vector2(a.x - b.x, a.y - b.y);
}

float Vector2::Dot(const Vector2 &a, const Vector2 &b){
    return (a.x * b.x) + (a.y + b.y);
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
