#include <stdbool.h>
//
// Created by oskar on 03-11-2025.
//
#ifndef FOREST_FIRE_PREVENTION_FUNCTIONS_H
#define FOREST_FIRE_PREVENTION_FUNCTIONS_H
typedef enum {empty, fresh, burning, burnt, wet} status_e;
typedef struct {
    status_e status;
    int humidity;
    double fuel_left;
} tree_t;
void make_rnd_forest(tree_t* forest, double density, int size);
void print_forest(tree_t* forest, int height, int width);
tree_t* get_tree(int x, int y, int width, tree_t* forest);
void scan_settings(int* width, int* height, double* density);

int chance(int procent,bool forest_thinning,bool is_wet);

void check_surrounding(tree_t* forest, int x, int y,int width, int height);
void user_drop_water(tree_t* forest, int x, int y, int width);
void user_dead_zone(tree_t* forest, int x, int y, int width, int size_of_dead_zone);


#endif //FOREST_FIRE_PREVENTION_FUNCTIONS_H