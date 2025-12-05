#include "wind.h"
#include "vector-math.h"
#include <math.h>
#include "probability.h"
#include <stdio.h>
#include <stdlib.h>
#include "fire-sim.h"

#define WIND_SPEED_MIN 0.5
#define WIND_SPEED_MAX 15

wind_t rnd_wind() {
    wind_t wind;
    double x, y, speed;
    do
    {
        x = (double)(rand() % (WIND_SPEED_MAX * 100 + 1)) / 100;
        y = (double)(rand() % (WIND_SPEED_MAX * 100 + 1)) / 100;
        speed = length_of_vector((vector_t){x, y});
    } while (!(WIND_SPEED_MIN <= speed && speed <= WIND_SPEED_MAX));

    if (random_chance(50))
    {
        x *= -1;
    }
    if (random_chance(50))
    {
        y *= -1;
    }
    update_wind(&wind, x, y);
    return wind;
}

void update_wind(wind_t* wind, double x, double y) {
    update_wind_direction(wind, x, y);
    update_wind_speed(wind);
}
void print_wind(wind_t* wind) {
    //Array af strenge til at match med direction samt Unicode symbol for retning
    const char* direction[8] = {"EAST",
                                "NORTH EAST",
                                "NORTH",
                                "NORTH WEST",
                                "WEST",
                                "SOUTH WEST",
                                "SOUTH",
                                "SOUTH EAST"};
    double direction_index = angle_between_vectors(wind->direction, (vector_t){1, 0});
    if (wind->direction.y < 0)
        direction_index = 2 * M_PI - direction_index;
    direction_index = (int)round(direction_index / (M_PI / 4)) % 8;
    printf("Wind direction: %s\n",direction[(int)direction_index]);
    printf("Wind speed: %lf m/s\n",wind->speed);
    printf("%f, %f\n", wind->direction.x, wind->direction.y);
}
void update_wind_direction(wind_t* wind, double x, double y) {
    wind->direction.x = x;
    wind->direction.y = y;
}
void update_wind_speed(wind_t* wind) {
    wind->speed = length_of_vector(wind->direction);
}