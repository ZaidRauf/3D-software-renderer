#pragma once

#include <cmath>
#include <iostream>

class Vector2{
    public:
        float x;
        float y;
        
        Vector2();
        Vector2(const float x, const float y);
        Vector2(const Vector2 &vector);
        ~Vector2();

        static Vector2 Add(const Vector2 &a, const Vector2 &b);
        static Vector2 Subtract(const Vector2 &a, const Vector2 &b);
        static float Dot(const Vector2 &a, const Vector2 &b);
        static Vector2 ScalarMultiply(const float &scalar, const Vector2 &vector);
        Vector2 operator=(const Vector2 &v);
};

Vector2 operator+(const Vector2 &a, const Vector2 &b);
Vector2 operator-(const Vector2 &a, const Vector2 &b);
float operator*(const Vector2 &a, const Vector2 &b);
Vector2 operator*(const float &scalar, const Vector2 &vector);
Vector2 operator*(const Vector2 &vector, const float &scalar);
