#ifndef FOREST_FIRE_PREVENTION_WIND_H
#define FOREST_FIRE_PREVENTION_WIND_H
typedef enum {NORTH, EAST, SOUTH, WEST} wind_e;
typedef struct {
    wind_e direction;
    double speed;
} wind_t;
void start_wind(wind_t* wind);
void print_wind(wind_t* wind);
void update_wind(wind_t* wind);
void update_wind_direction(wind_t* wind);
void update_wind_speed(wind_t* wind);

#endif //FOREST_FIRE_PREVENTION_WIND_H
