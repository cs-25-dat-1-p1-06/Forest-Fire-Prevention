#ifndef FOREST_FIRE_PREVENTION_MORE_MATH_H
#define FOREST_FIRE_PREVENTION_MORE_MATH_H
typedef struct
{
    double x, y;
    double length;
} vector_t;

vector_t new_vector(double x, double y);
double length_of_vector(vector_t);
double dot_product_vectors(vector_t v1, vector_t v2);
vector_t scalar_product_vector(vector_t vector, double factor);
double angle_between_vectors(vector_t v1, vector_t v2);

#endif //FOREST_FIRE_PREVENTION_MORE_MATH_H