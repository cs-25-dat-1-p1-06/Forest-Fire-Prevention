//
// Created by nickl on 05-12-2025.
//

#include "probability.h"
#include <stdlib.h>
#include "math.h"
#include "fire-sim.h"


int random_chance(int chance)
{
    return rand() % 100 + 1 <= chance;
}
double heat_from_distance(tree_t tree, double distance){
    return heat_by_fuel_left(tree) / (pow(distance, 2) * 2);
}
double heat_prob(double heat) {
    return 1 - heat;
}
double wind_prob(wind_t wind) {
}
double distance_given_coord(int a, int b) {
    return sqrt(pow(a, 2) + pow(b, 2));
}
double heat_by_fuel_left(tree_t tree) {
    if (tree.status != burning) return 0;

    double my = STARTING_TREE_FUEL/2;
    double x = tree.fuel_left;
    double sigma = 0.2;
    double heat = 1;
    heat *= 1 / sqrt(2 * M_PI * pow(sigma,2));
    heat *= pow(M_E,- pow(x - my,2) / 2 * pow(sigma,2));
    heat *= HEAT_FACTOR;
    return heat;
}
