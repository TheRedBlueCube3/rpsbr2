#ifndef VECTOR_H
#define VECTOR_H

typedef struct
{
    float x, y;    
} Vector;

extern Vector g_vecZero;

Vector VEC_Add(Vector, Vector);
Vector VEC_Sub(Vector, Vector);
Vector VEC_Mult(Vector, float);
Vector VEC_Div(Vector, float);
Vector VEC_Negate(Vector);
Vector VEC_Pow(Vector, float);
Vector VEC_InvSquare(Vector vec1, Vector vec2, float dist2);
float  VEC_Dist2(Vector, Vector);

Vector VEC_Limit(Vector, float limit);
float  VEC_Magnitude(Vector);
Vector VEC_SetMagnitude(Vector, float mag);
Vector VEC_Normalize(Vector);

#endif