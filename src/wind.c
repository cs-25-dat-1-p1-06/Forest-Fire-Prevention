#include "wind.h"

#include <stdio.h>
#include <stdlib.h>

#define WIND_SPEED_MIN 0.5
#define WIND_SPEED_MAX 15.0

void start_wind(wind_t* wind) {
    update_wind(wind);
}

void update_wind(wind_t* wind) {
    update_wind_direction(wind);
    update_wind_speed(wind);
}
void print_wind(wind_t* wind) {
    const char* direction[4] = {"NORTH","EAST","SOUTH","WEST"}; //Char array til at match med direction
    printf("Wind direction: %s\n",direction[wind->direction]);
    printf("Wind speed: %lf m/s\n",wind->speed);
}
void update_wind_direction(wind_t* wind) {
    wind->direction = rand()%4; //Tilfældig direktion mellem (0-3)
}
void update_wind_speed(wind_t* wind) {
    //Tilfældig speed mellem max og min.
    double randomSpeed = ((double) rand() / RAND_MAX) * (WIND_SPEED_MAX - WIND_SPEED_MIN) + WIND_SPEED_MIN;

    wind->speed =  randomSpeed;
}