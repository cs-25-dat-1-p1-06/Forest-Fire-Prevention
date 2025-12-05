//
// Created by nickl on 04-12-2025.
//


#include "vector-math.h"


#include "math.h"

double length_of_vector(vector_t vector)
{
    return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

double dot_product_vectors(vector_t v1, vector_t v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

void scalar_product_vector(vector_t* vector, double factor)
{
    vector->x *= factor;
    vector->y *= factor;
}

double angle_between_vectors(vector_t v1, vector_t v2)
{
    return acos(dot_product_vectors(v1, v2) / length_of_vector(v1) * length_of_vector(v2));
}
