#ifndef FOREST_FIRE_PREVENTION_WIND_H
#define FOREST_FIRE_PREVENTION_WIND_H
#include "vector-math.h"

/**
 * Laver en tilfældig vind
 */
vector_t rnd_wind();

/**
 * Udskriver vindens retning samt hastighed
 * @param wind vinden der skal udskrives
 */
void print_wind(vector_t wind);

/**
 * Finder et indeks i urets retning ud fra vinklen mellem en vektor og vektoren j (0,1)
 * @param v1 vektoren
 * @param array_size mængden af elementer som kan vælges fra
 * @return et indeks til et element ud fra hvor vektoren peger i urets retning
 */
int clockwise_position(vector_t v1, int array_size);

#endif //FOREST_FIRE_PREVENTION_WIND_H
