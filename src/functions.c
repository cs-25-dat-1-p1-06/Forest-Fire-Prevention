//
// Created by oskar on 03-11-2025.
//

#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

void make_rnd_forest(tree_t* forest, double density, int size) {
    for (int i = 0; i < size; i++) {
        forest[i].status = rand() % 2;
    }
}
void print_forest(tree_t* forest, int height, int width) {
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            switch (get_tree(i,j,width,forest).status) {
                case empty:
                    printf("0");
                    break;
                case fresh:
                    printf("1");
                    break;
                case burning:
                    printf("2");
                    break;
                case burnt:
                    printf("3");
                    break;
            }
        }
        printf("\n");
    }
}
tree_t get_tree(int x, int y, int width, tree_t* forest) {
    return forest[width * y + x];
}

// a function to return a prosent chance based on different factors

 int chance(int procent,bool forest_thinning,bool is_wet) {

    if (forest_thinning) {
        procent = procent-25;
    }else procent = procent+5;

    if (is_wet) {
        procent = procent-20;
    }else procent = procent+5;
    printf("%d \n",procent);


    return rand() % 100 < procent;
}

tree_t* check_surrounding(tree_t* forest, tree_t* surrounding, int x, int y,int width, int height) {

    int counter = 0;
    for (int j = -1; j <= 1; j++) {
        for (int i = -1; i <= 1; i++) {
            surrounding[counter] = get_tree(x+i,y+j,width,forest);
            printf("%d",surrounding[counter].status);
            counter++;
        }
        printf("\n");
    }
return 0;
}
