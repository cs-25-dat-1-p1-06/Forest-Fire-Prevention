#ifndef FOREST_FIRE_PREVENTION_MORE_MATH_H
#define FOREST_FIRE_PREVENTION_MORE_MATH_H
typedef struct
{
    double x, y;
} vector_t;

double length_of_vector(vector_t);
double dot_product_vectors(vector_t v1, vector_t v2);
void scalar_product_vector(vector_t* vector, double factor);
double angle_between_vectors(vector_t v1, vector_t v2);

#endif //FOREST_FIRE_PREVENTION_MORE_MATH_H