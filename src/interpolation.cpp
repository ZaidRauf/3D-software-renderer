#include "interpolation.h"
#include <cmath>

Vector3 interpolation::barycentric_weights(const Vector2 &a, const Vector2 &b, const Vector2 &c, const Vector2 &p){
    
    float triangle_area = std::fabs(Determinant(a - b, a - c));

    float alpha = std::fabs(Determinant(p - b, p - c))/triangle_area;
    float beta = std::fabs(Determinant(p - a, p - c))/triangle_area;
    float gamma = 1.0 - alpha - beta;

    return Vector3(alpha, beta, gamma);
}
