#include "wind.h"
#include "vector-math.h"
#include <math.h>
#include "probability.h"
#include <stdio.h>
#include <stdlib.h>

#define WIND_SPEED_MIN 10
#define WIND_SPEED_MAX 15

vector_t rnd_wind() {
    vector_t wind;
    double x, y;

    //laver en tilfældig vind indenfor min speed og max speed
    do
    {
        x = (double)(rand() % (WIND_SPEED_MAX * 100 + 1)) / 100;
        y = (double)(rand() % (WIND_SPEED_MAX * 100 + 1)) / 100;
        wind = new_vector(x, y);
    } while (!(WIND_SPEED_MIN <= wind.length && wind.length <= WIND_SPEED_MAX));

    //50:50 for at vi vender vektoren på x aksen
    if (random_chance(0.5))
    {
        x *= -1;
    }
    //50:50 for at vi vender vektoren på y aksen
    if (random_chance(0.5))
    {
        y *= -1;
    }

    return wind;
}

void print_wind(vector_t wind) {
    //Array af strenge til at match med direction samt Unicode symbol for retning
    const char* direction[8] = {"NORTH",
                                "NORTH EAST",
                                "EAST",
                                "SOUTH EAST",
                                "SOUTH",
                                "SOUTH WEST",
                                "WEST",
                                "NORTH WEST"
                                };
    printf("Wind direction: %s\n",direction[clockwise_position(wind, 8)]);
    printf("Wind speed: %lf m/s\n",wind.length);
    printf("%f, %f\n", wind.x, wind.y);
}

int clockwise_position(vector_t v1, int array_size)
{
    double direction_index = angle_between_vectors(v1, (vector_t){0, 1});
    //arbejder altid med den store vinkel
    if (v1.x < 0)
        direction_index = 2 * M_PI - direction_index;
    direction_index = (int)round(direction_index / (M_PI / (array_size / 2))) % array_size;
    return direction_index;
}