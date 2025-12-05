#ifndef FOREST_FIRE_PREVENTION_WIND_H
#define FOREST_FIRE_PREVENTION_WIND_H
#include "vector-math.h"

/**
 * Initialiserer wind_t
 * @param wind Pointer til en vindstruktur der skal initialiseres
 */
vector_t rnd_wind();

/**
 * Udskriver vindens direktion samt hastighed
 * @param wind Pointer til vindstruktur der skal udskrives
 */
void print_wind(vector_t wind);
int clockwise_position(vector_t v1, int array_size);

#endif //FOREST_FIRE_PREVENTION_WIND_H
