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
    return chance_limiter(heat / pow(distance, 2));
}

double wind_prob(vector_t wind, vector_t position) {
    double angle = angle_between_vectors(wind, position);

    //apart er en værdi mellem 0 og 1 som kommer tættere på 1 jo større vinklen mellem de to vektorer er, og når maksimum ved v = pi rad
    double apart = angle / M_PI;
    double close = 1 - apart;

    //lav vinkel og høj vind = højere sandsynlighed
    //lav vinkel og lav vind = lavere sandsynlighed
    //høj vinkel og høj vind = lavere sandsynlighed
    //lav vind = gennemsnitlig sandsynlighed
    double probability = (1 - close / wind.length) * (1 - apart * wind.length);

    //jo længere væk, jo mindre en effekt fra vind
    // probability /= position.length;

    //en basis værdi for sandsynligheden, jo lavere vindens hastighed er, jo mere nærmes denne værdi
    // probability += 0.5;

    return chance_limiter(probability);
}

double humidity_not_prob(tree_t tree) {
    return tree.humidity * 0.01;
}