//
// Created by oskar on 03-11-2025.
//
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

int main(void) {
    srand(time(NULL));
    int width, height;
    double density;
    scan_settings(&width, &height, &density);
    tree_t* forest = malloc(width*height*sizeof(tree_t));
    make_rnd_forest(forest, density, width*height);
    print_forest(forest, height, width);
    int x = 2;
    int y = 1;

    //Vi printer træets koordinater ud fra at øverste venstre hjørne, er (0,0) og printer dets værdi
    printf("Tree at (%d,-%d) has the status %d\n", x, y, get_tree(x,y,width,forest)->status );
    printf("%d\n", forest[width*y+x].status);
    chance(100,1,0);
    printf("The statuses of the trees surrounding (x,y) are the following\n");
    check_surrounding(forest, x,y,width,height);
    printf("You have dropped water on the area surrounding (x,y)\n");
    user_drop_water(forest, x, y, width);
    printf("You have made a dead zone of 3 trees in each direction around (x+2,y+2)\n");
    user_dead_zone(forest,x+2,y+2,width,3);
    print_forest(forest, height, width);
    free(forest);


    return 0;
}