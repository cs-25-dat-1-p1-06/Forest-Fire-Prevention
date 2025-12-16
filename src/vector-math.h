#ifndef FOREST_FIRE_PREVENTION_MORE_MATH_H
#define FOREST_FIRE_PREVENTION_MORE_MATH_H
//struct til vektorer, længden er gemt da x og y er til en enhedsvektor
typedef struct
{
    double x, y;
    double length;
} vector_t;

/**
 * skaber en ny vektor
 * @param x x værdi til vektor
 * @param y y værdi til vektor
 * @return returnerer en ny vektor som enhedsvektor og den aktuelle længde gemt
 */
vector_t new_vector(double x, double y);

/**
 * Finder længden af en vektor
 * @param x x værdi til vektor
 * @param y y værdi til vektor
 * @return længden af vektoren
 */
double length_of_vector(double x, double y);

/**
 * Finder prikproduktet af to vektorer
 * @param v1 vektor 1
 * @param v2 vektor 2
 * @return prikproduktet af de to vektorer
 */
double dot_product_vectors(vector_t v1, vector_t v2);

/**
 * Finder en skalar vektor baseret på en vektor og skalarfaktor
 * @param vector vektoren
 * @param factor skalarfaktoren
 * @return skalarvektoren
 */
vector_t scalar_product_vector(vector_t vector, double factor);

/**
 * Finder vinklen imellem to vektorer
 * @param v1 vektor 1
 * @param v2 vektor 2
 * @return vinklen imellem de to vektorer
 */
double angle_between_vectors(vector_t v1, vector_t v2);

#endif //FOREST_FIRE_PREVENTION_MORE_MATH_H