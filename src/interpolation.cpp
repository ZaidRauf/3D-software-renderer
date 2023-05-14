#include "interpolation.h"
#include <cmath>

Vector3 interpolation::barycentric_weights(const Vector2 &a, const Vector2 &b, const Vector2 &c, const Vector2 &p){
    
    float triangle_det = linalg::Determinant(a - c, b - c);
    float alpha = ((b.y - c.y)*(p.x - c.x) + (c.x - b.x)*(p.y - c.y))/triangle_det;
    float beta = ((c.y - a.y)*(p.x - c.x) + (a.x - c.x)*(p.y - c.y))/triangle_det;
    float gamma = 1.0 - alpha - beta;

    return Vector3(alpha, beta, gamma);
}
