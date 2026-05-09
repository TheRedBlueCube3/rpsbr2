#include "Vector.h"
#include <math.h>
#include <stdio.h>
Vector g_vecZero = {0.0f, 0.0f};

Vector VEC_Add(Vector vec1, Vector vec2)
{
    return (Vector) {vec1.x + vec2.x, vec1.y + vec2.y};
}
Vector VEC_Sub(Vector vec1, Vector vec2)
{
    return (Vector) {vec1.x - vec2.x, vec1.y - vec2.y};
}

Vector VEC_Mult(Vector vec, float scalar)
{
    return (Vector) {vec.x * scalar, vec.y * scalar};
}
Vector VEC_Div(Vector vec, float scalar)
{
    return (Vector) {vec.x / scalar, vec.y / scalar};
}

Vector VEC_Negate(Vector vec)
{
    return (Vector) {-vec.x, -vec.y};
}

Vector VEC_Pow(Vector vec, float power)
{
    return (Vector) {pow(vec.x, power), pow(vec.y, power)};
}

Vector VEC_InvSquare(Vector thisPosition, Vector otherPosition, float dist2)
{
        static int count = 0;
    if(dist2 < 0.01f && count++ < 5) {
        printf("VEC_InvSquare: dist2=%f, this=(%f,%f), other=(%f,%f)\n", 
               dist2, thisPosition.x, thisPosition.y, otherPosition.x, otherPosition.y);
    }
    if(dist2 < 0.0001f) return g_vecZero;  // Avoid division by zero
    float r = sqrtf(dist2);
    float r3 = dist2 * r;
    return VEC_Div(VEC_Sub(thisPosition, otherPosition), r3);
}

float VEC_Dist2(Vector vec1, Vector vec2)
{
    float diff1 = vec1.x - vec2.x;
    float diff2 = vec1.y - vec2.y;
    return diff1 * diff1 + diff2 * diff2;
}

float VEC_Magnitude(Vector vec)
{
    return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

Vector VEC_SetMagnitude(Vector vec, float mag)
{
    float currMag = VEC_Magnitude(vec);
    static int count = 0;
    if(mag < 0.01f && count++ < 5) {
        printf("VEC_Normalize: mag=%f, vec=(%f,%f)\n", mag, vec.x, vec.y);
    }
    if(currMag < 0.0001f)
    {
        return g_vecZero;
    }

    return VEC_Mult(vec, mag / currMag);
}

Vector VEC_Normalize(Vector vec)
{
    return VEC_SetMagnitude(vec, 1);
}

Vector VEC_Limit(Vector vec, float limit)
{
    float currMag = VEC_Magnitude(vec);
    if(currMag < limit)
    {
        return vec;
    }
    return VEC_SetMagnitude(vec, limit);
}