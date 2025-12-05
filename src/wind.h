#ifndef FOREST_FIRE_PREVENTION_WIND_H
#define FOREST_FIRE_PREVENTION_WIND_H
#include "vector-math.h"
// Struct der beskriver vind med retning og hastighed
typedef struct {
    vector_t direction; // vindretning defineret som retningsvektor
    double speed; // Vindhastighed i passende enhed (meter pr sekund), kan ganges med vektoren for at få den fulde vektor
} wind_t;

/**
 * Initialiserer wind_t
 * @param wind Pointer til en vindstruktur der skal initialiseres
 */
wind_t rnd_wind();

/**
 * Udskriver vindens direktion samt hastighed
 * @param wind Pointer til vindstruktur der skal udskrives
 */
void print_wind(wind_t* wind);

/**
 * Opdaterer vindens direktion og hastighed tilfældigt.
 * @param wind Pointer til vindstruktur der skal opdateres
 */
void update_wind(wind_t* wind, double x, double y);

/**
 * Opdaterer vindens direktion tilfældigt
 * @param wind Pointer til vindstruktur der skal opdateres
 */
void update_wind_direction(wind_t* wind, double x, double y);

/**
 * Opdaterer vindens hastighed tilfældigt
 * @param wind Pointer til vindstruktur der skal opdateres
 */
void update_wind_speed(wind_t* wind);

#endif //FOREST_FIRE_PREVENTION_WIND_H
