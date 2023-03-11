#pragma once

#include <cmath>
#include <iostream>


// Need to decalre above 
class Vector3;
class Vector4;

// Vector2 
class Vector2{
    public:
        float x;
        float y;
        
        Vector2();
        Vector2(const float x, const float y);
        Vector2(const Vector2 &vector);
        Vector2(const Vector3 &vector);
        ~Vector2();
        float Length();
        Vector2 Normalized();

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
Vector2 operator*(const float &scalar, const Vector2 &vector);
Vector2 operator*(const Vector2 &vector, const float &scalar);
std::ostream& operator<<(std::ostream &os, const Vector2 &v);

// Vector3 
class Vector3{
    public:
        float x;
        float y;
        float z;

        Vector3();
        Vector3(const float x, const float y, float z);
        Vector3(const Vector2 &vector);
        Vector3(const Vector3 &vector);
        ~Vector3();
        float Length();
        Vector3 Normalized();

        static Vector3 Add(const Vector3 &a, const Vector3 &b);
        static Vector3 Subtract(const Vector3 &a, const Vector3 &b);
        static float Dot(const Vector3 &a, const Vector3 &b);
        static Vector3 ScalarMultiply(const float &scalar, const Vector3 &vector);
        Vector3 operator=(const Vector3 &v);
};

Vector3 operator+(const Vector3 &a, const Vector3 &b);
Vector3 operator-(const Vector3 &a, const Vector3 &b);
float operator*(const Vector3 &a, const Vector3 &b);
Vector3 operator*(const float &scalar, const Vector3 &vector);
Vector3 operator*(const Vector3 &vector, const float &scalar);
Vector3 operator*(const float &scalar, const Vector3 &vector);
Vector3 operator*(const Vector3 &vector, const float &scalar);
std::ostream& operator<<(std::ostream &os, const Vector3 &v);

// Vector4 
class Vector4{
    public:
        float x;
        float y;
        float z;
        float w;

        Vector4();
        Vector4(const float x, const float y, float z, float w);
        Vector4(const Vector2 &vector);
        Vector4(const Vector3 &vector);
        Vector4(const Vector4 &vector);
        ~Vector4();
        float Length();
        Vector4 Normalized();

        static Vector4 Add(const Vector4 &a, const Vector4 &b);
        static Vector4 Subtract(const Vector4 &a, const Vector4 &b);
        static float Dot(const Vector4 &a, const Vector4 &b);
        static Vector4 ScalarMultiply(const float &scalar, const Vector4 &vector);
        Vector4 operator=(const Vector4 &v);
};

Vector4 operator+(const Vector4 &a, const Vector4 &b);
Vector4 operator-(const Vector4 &a, const Vector4 &b);
float operator*(const Vector4 &a, const Vector4 &b);
Vector4 operator*(const float &scalar, const Vector4 &vector);
Vector4 operator*(const Vector4 &vector, const float &scalar);
Vector4 operator*(const float &scalar, const Vector4 &vector);
Vector4 operator*(const Vector4 &vector, const float &scalar);
std::ostream& operator<<(std::ostream &os, const Vector4 &v);
