#include "vector-math.h"
#include "math.h"


vector_t new_vector(double x, double y)
{
    vector_t vector;
    vector.x = x;
    vector.y = y;

    vector.length = length_of_vector(vector);

    return scalar_product_vector(vector, 1 / vector.length);
}

double length_of_vector(vector_t vector)
{
    return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

double dot_product_vectors(vector_t v1, vector_t v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

vector_t scalar_product_vector(vector_t vector, double factor)
{
    vector_t scalar_product = vector;
    scalar_product.x *= factor;
    scalar_product.y *= factor;

    return scalar_product;
}

double angle_between_vectors(vector_t v1, vector_t v2)
{
    return acos(dot_product_vectors(v1, v2) / length_of_vector(v1) * length_of_vector(v2));
}
