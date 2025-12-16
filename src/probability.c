#include "probability.h"
#include <stdlib.h>
#include "math.h"
#include "fire-sim.h"


int random_chance(double chance)
{
    return (double)(rand() % 100 + 1) / 100 <= chance;
}

double chance_limiter(double chance)
{
    chance = chance < 0 ? 0 : chance;
    chance = 1 < chance ? 1 : chance;
    return chance;
}

double heat_prob(double heat, double distance) {
    return chance_limiter(heat / pow(distance, 3));
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
    probability /= pow(position.length,3);

    //en basis værdi for sandsynligheden, jo lavere vindens hastighed er, jo mere nærmes denne værdi
    // probability += 0.5;

    return chance_limiter(probability);
}

double humidity_not_prob(tree_t tree) {
    return tree.humidity * 0.01;
}

double distance_given_coord(int a, int b) {
    return sqrt(pow(a, 2) + pow(b, 2));
}

void heat_by_fuel_left(tree_t* tree) {
    if (tree->status != burning) tree->heat = 0;
    //Vi bruger formlen for en parabel med et variabelt toppunkt.
    double top_x = TREE_FUEL / 2;
    double top_y = MAX_HEAT;
    double b = 4 * top_y / (2 * top_x);
    double a = -b / (2 * top_x);
    tree->heat = a * pow(tree->fuel_left,2) + b * tree->fuel_left;
}
