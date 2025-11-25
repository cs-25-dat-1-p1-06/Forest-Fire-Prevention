#ifndef FOREST_FIRE_PREVENTION_FIRESIM_H
#define FOREST_FIRE_PREVENTION_FIRESIM_H
#include <windows.h>
#include <wind.h>

typedef enum {empty, fresh, burning, burnt, wet} status_e;
typedef struct {
    status_e status;
    int humidity, fire_strength;
    double fuel_left;
} tree_t;

void make_rnd_forest(tree_t* forest, double density, int size);
void print_forest(tree_t* forest, int height, int width);
tree_t* get_tree(int x, int y, int width, tree_t* forest);
int get_trees_amount(tree_t* forest, int size, status_e target);
void status_text(wind_t* wind, tree_t* forest, int size);

int sim_finished_check(tree_t* forest, int size);

int check_surrounding_firestrength(tree_t* forest, int x, int y,int width, int height);
void user_drop_water(tree_t* forest, int x, int y, int width);
void user_dead_zone(tree_t* forest, int x, int y, int width, int size_of_dead_zone);
void tick(tree_t* forest, int height, int width);
void burndown(tree_t* forest, int height, int width);
void fire_spread(tree_t* forest, int height, int width);
int check_surrounding_burning(tree_t* forest, int width, int x, int y);
int calculate_risk_of_burning();





#endif //FOREST_FIRE_PREVENTION_FIRESIM_H