//
// Created by nickl on 05-12-2025.
//

#ifndef FOREST_FIRE_PREVENTION_PROBABILITY_H
#define FOREST_FIRE_PREVENTION_PROBABILITY_H
#include "fire-sim.h"

int random_chance(int chance);
double heat_from_distance(tree_t tree, double distance);
double distance_given_coord(int a, int b);
double heat_prob(double heat);
double heat_by_fuel_left(tree_t tree);

#endif //FOREST_FIRE_PREVENTION_PROBABILITY_H