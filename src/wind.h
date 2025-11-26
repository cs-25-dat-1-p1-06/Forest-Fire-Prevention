#ifndef FOREST_FIRE_PREVENTION_WIND_H
#define FOREST_FIRE_PREVENTION_WIND_H

// Enum over de fire mulige vindretninger
typedef enum {NORTH, EAST, SOUTH, WEST} wind_e;

// Struct der beskriver vind med retning og hastighed
typedef struct {
    wind_e direction;   // Aktuel vindretning
    double speed;       // Vindhastighed i passende enhed (meter pr sekund)
} wind_t;

/**
 * Initialiserer wind_t
 * @param wind Pointer til en vindstruktur der skal initialiseres
 */
void start_wind(wind_t* wind);

/**
 * Udskriver vindens direktion samt hastighed
 * @param wind Pointer til vindstruktur der skal udskrives
 */
void print_wind(wind_t* wind);

/**
 * Opdaterer vindens direktion og hastighed tilfældigt.
 * @param wind Pointer til vindstruktur der skal opdateres
 */
void update_wind(wind_t* wind);

/**
 * Opdaterer vindens direktion tilfældigt
 * @param wind Pointer til vindstruktur der skal opdateres
 */
void update_wind_direction(wind_t* wind);

/**
 * Opdaterer vindens hastighed tilfældigt
 * @param wind Pointer til vindstruktur der skal opdateres
 */
void update_wind_speed(wind_t* wind);

#endif //FOREST_FIRE_PREVENTION_WIND_H
