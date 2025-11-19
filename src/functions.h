#ifndef FOREST_FIRE_PREVENTION_FUNCTIONS_H
#define FOREST_FIRE_PREVENTION_FUNCTIONS_H
#include <windows.h>

typedef enum {empty, fresh, burning, burnt} status_e;
typedef struct {
    status_e status;
    int humidity;
    double fuel_left;
} tree_t;
void make_rnd_forest(tree_t* forest, double density, int size);
void print_forest(tree_t* forest, int height, int width);
tree_t get_tree(int x, int y, int width, tree_t* forest);
void scan_settings(int* width, int* height, double* density);
void color_change(unsigned short color);
int chance(int procent, int forest_thinning, int is_wet);
int MouseEventProc(MOUSE_EVENT_RECORD mer, int *x, int *y);
void user_click_input(int *x, int *y);
void console_setup();



tree_t* check_surrounding(tree_t* forest, tree_t* surrounding, int x, int y,int width, int height);


#endif //FOREST_FIRE_PREVENTION_FUNCTIONS_H