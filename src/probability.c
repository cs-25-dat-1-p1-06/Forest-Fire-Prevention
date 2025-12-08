#include "probability.h"
#include <stdlib.h>
#include "math.h"
#include "fire-sim.h"


int random_chance(double chance)
{
    if (chance <= 0 || chance >= 1)
        return (int)chance_limiter(chance);
    return (double)(rand() % 101) / 100 <= chance;
}

double chance_limiter(double chance)
{
    chance = chance < 0 ? 0 : chance;
    chance = 1 < chance ? 1 : chance;
    return chance;
}
double heat_from_distance(double heat, double distance){
    return heat / pow(distance, 2);
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

void heat_by_fuel_left(tree_t* tree) {
    if (tree->status != burning) tree->heat = 0;
    //Vi bruger formlen for en parabel med et variabelt toppunkt.
    double x = tree->fuel_left;
    int c = 0;
    double top_x = TREE_FUEL/2;
    double top_y = MAX_HEAT;
    double b = -((c - top_y ) * 4) / 2 * top_x;
    double a = -b / 2 * top_x;
    tree->heat = a * pow(x,2) + b * x + c;
}
