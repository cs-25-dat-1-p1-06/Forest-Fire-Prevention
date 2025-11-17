//
// Created by oskar on 03-11-2025.
//
#include "functions.h"
#include "wind.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>




int main(void) {

    srand(time(NULL));
    /*
    tree_t* surrounding = malloc(9*sizeof(tree_t));
    int width, height;
    double density;
    scan_settings(&width, &height, &density);
    tree_t* forest = malloc(width*height*sizeof(tree_t));
    make_rnd_forest(forest, density, width*height);
    print_forest(forest, height, width);
    int x = 2;
    int y = 1;
    */

    //Initiliasere vind
    struct Wind *wind = (struct Wind*) malloc(sizeof(struct Wind));

    //Tjekker om allokeringen var en success
    if (wind == NULL) {
        printf("Der er ikke plads i hukommelsen!");
    }
    start_wind(wind);
    update_wind(wind);
    print_wind(wind);
    update_wind(wind);
    print_wind(wind);

    /*
    //Vi printer træets koordinater ud fra at øverste venstre hjørne, er (0,0) og printer dets værdi
    printf("Tree at (%d,-%d) has the status %d\n", x, y, get_tree(x,y,width,forest).status );
    printf("%d\n", forest[width*y+x].status);
    chance(100,1,0);
    printf("The statuses of the trees surrounding (x,y) are the following\n");

    surrounding = check_surrounding(forest, surrounding, x,y,width,height);
    free(forest);
    free(surrounding);
    */
    free(wind);

    return 0;
}