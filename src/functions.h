#ifndef FOREST_FIRE_PREVENTION_FUNCTIONS_H
#define FOREST_FIRE_PREVENTION_FUNCTIONS_H
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
int get_trees_amount(tree_t* forest, int height, int width,const char* status_str);
void scan_settings(int* width, int* height, double* density);
void color_change(unsigned short color);

int MouseEventProc(MOUSE_EVENT_RECORD mer, int *x, int *y);
void user_click_input(int *x, int *y);
void console_setup();


void chance(tree_t *tree);

void check_surrounding(tree_t* forest, int x, int y,int width, int height);
void user_drop_water(tree_t* forest, int x, int y, int width);
void user_dead_zone(tree_t* forest, int x, int y, int width, int size_of_dead_zone);

void status_text(wind_t* wind, tree_t* forest, int height, int width);

#endif //FOREST_FIRE_PREVENTION_FUNCTIONS_H