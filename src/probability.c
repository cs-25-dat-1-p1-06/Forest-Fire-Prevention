#include "probability.h"
#include <stdlib.h>
#include "math.h"
#include "fire-sim.h"


int random_chance(int chance)
{
    return rand() % 100 + 1 <= chance;
}

double chance_limiter(double chance)
{
    chance = chance < 0 ? 0 : chance;
    chance = 1 < chance ? 1 : chance;
    return chance;
}
double heat_from_distance(tree_t tree, double distance){
    return heat_by_fuel_left(tree) / (pow(distance, 2) * 2);
}
double heat_prob(double heat) {
    return chance_limiter(1 - heat);
}
double wind_prob(vector_t wind, vector_t position) {
    double angle = acos(dot_product_vectors(wind, position));
    //apart er en værdi mellem 0 og 1 som kommer tættere på 1 jo større vinklen mellem de to vektorer er, og når maksimum ved v = pi rad
    double apart = angle / M_PI;
    double close = 1 - apart;

    //lav vinkel og høj vind = højere sandsynlighed
    //lav vinkel og lav vind = lavere sandsynlighed
    //høj vinkel og høj vind = lavere sandsynlighed
    //lav vind = gennemsnitlig sandsynlighed
    double probability = (1 - close / wind.length) * (1 - apart * wind.length);

    //jo længere væk, jo mindre en effekt fra vind
    probability /= length_of_vector(position);

    //en basis værdi for sandsynligheden, jo lavere vindens hastighed er, jo mere nærmes denne værdi
    // probability += 0.5;

    return chance_limiter(1 - probability);
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
