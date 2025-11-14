//
// Created by oskar on 03-11-2025.
//
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>



int main(void) {
    srand(time(NULL));
    int width, height;
    double density;
    scan_settings(&width, &height, &density);

    tree_t* forest = malloc(width*height*sizeof(tree_t));
    make_rnd_forest(forest, 1, width*height);
    print_forest(forest, height, width);
    int x = 2;
    int y = 1;

    //Vi printer træets koordinater ud fra at øverste venstre hjørne, er (0,0) og printer dets værdi
    printf("Tree at (%d,-%d) has the status %d\n", x, y, get_tree(x,y,width,forest).status );
    printf("%d", forest[width*y+x].status);

    return 0;
}