#ifndef FOREST_FIRE_PREVENTION_PROBABILITY_H
#define FOREST_FIRE_PREVENTION_PROBABILITY_H
#include "fire-sim.h"

int random_chance(double chance);
double chance_limiter(double chance);
double heat_from_distance(double heat, double distance);
double distance_given_coord(int a, int b);
double heat_prob(double heat);
double wind_prob(vector_t wind, vector_t position);
double humidity_prob(tree_t tree);
void heat_by_fuel_left(tree_t* tree);

#endif //FOREST_FIRE_PREVENTION_PROBABILITY_H