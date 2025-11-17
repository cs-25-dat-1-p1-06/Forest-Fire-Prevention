#include "wind.h"

#include <stdio.h>
#include <stdlib.h>

#define WIND_SPEED_MIN 0.5
#define WIND_SPEED_MAX 15.0

void start_wind(struct Wind* wind) {
    update_wind(wind);
    print_wind(wind);
}

void update_wind(struct Wind* wind) {
    //Tilfældig speed mellem max og min.
    double randomSpeed = ((double) rand() / RAND_MAX) * (WIND_SPEED_MAX - WIND_SPEED_MIN) + WIND_SPEED_MIN;

    wind->speed =  randomSpeed;
    wind->direction = rand()%4; //Tilfældig direktion mellem (0-3)
}
void print_wind(struct Wind* wind) {
    const char* direction[4] = {"NORTH","EAST","SOUTH","WEST"}; //Char array til at match med direction
    printf("Direction is: %s\n",direction[wind->direction]);
    printf("Wind speed is: %lf\n",wind->speed);
}