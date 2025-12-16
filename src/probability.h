#ifndef FOREST_FIRE_PREVENTION_PROBABILITY_H
#define FOREST_FIRE_PREVENTION_PROBABILITY_H
#include "fire-sim.h"

/**
 * finder ud af om noget bør ske ud fra en sandsynlighed
 * @param chance sandsynligheden
 * @return returnerer 1 hvis et tilfældigt tal mellem 0.01 og 1 er under eller lig med sandsynligheden ellers 0
 */
int random_chance(double chance);

/**
 * begrænser værdien af en sandsynlighed
 * @param chance sandsynligheden
 * @return 1 hvis sandsynligheden > 1 og 0 hvis sandsynlighed < 0
 */
double chance_limiter(double chance);

/**
 * Finder sandsynligheden for en spredning baseret på varmen i de omkringliggende brændende træer
 * @param heat varmen i et træ
 * @param distance distancen fra origo træ
 * @return sandsynligheden for en spredning baseret på varmen
 */
double heat_prob(double heat, double distance);

/**
 * Finder sandsynligheden for en spredning baseret på vinden
 * @param wind vinden
 * @param position vektor der peger i retning af spredningen
 * @return sandsynligheden for en spredning baseret på vinden
 */
double wind_prob(vector_t wind, vector_t position);

/**
 * sandsynligheden for at en spredning ikke sker baseret på fugtigheden i origo træ
 * @param tree origo træet
 * @return sandsynligheden for at der ikke sker en spredning baseret på fugtigheden
 */
double humidity_not_prob(tree_t tree);

#endif //FOREST_FIRE_PREVENTION_PROBABILITY_H