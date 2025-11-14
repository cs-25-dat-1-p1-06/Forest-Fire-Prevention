//
// Created by oskar on 03-11-2025.
//
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define WIDTH 10
#define HEIGHT 5


int main(void) {
    srand(time(NULL));

    tree_t* forest = malloc(WIDTH*HEIGHT*sizeof(tree_t));
    make_rnd_forest(forest, 1, WIDTH*HEIGHT);
    print_forest(forest, HEIGHT, WIDTH);
    int x = 2;
    int y = 1;

    //Vi printer træets koordinater ud fra at øverste venstre hjørne, er (0,0) og printer dets værdi
    printf("Tree at (%d,-%d) has the status %d\n", x, y, get_tree(x,y,WIDTH,forest).status );
    printf("%d", forest[WIDTH*y+x].status);

    return 0;
}