//
// Created by oskar on 03-11-2025.
//

#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#define MAX_WIDTH 500
#define MAX_HEIGHT 100


void make_rnd_forest(tree_t* forest, double density, int size) {
    for (int i = 0; i < size; i++) {
        if (rand() % 100 < density*100){
            forest[i].status = 1;
        }
        else {
            forest[i].status = 0;
        }
    }
}
void print_forest(tree_t* forest, int height, int width) {
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            switch (get_tree(i,j,width,forest)->status) {
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
                case wet:
                    printf("4");
                    break;
            }
        }
        printf("\n");
    }
}
tree_t* get_tree(int x, int y, int width, tree_t* forest) {
    return &forest[width * y + x];
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

void check_surrounding(tree_t* forest, int x, int y,int width, int height) {
    int counter = 0;
    tree_t* surrounding = malloc(9*sizeof(tree_t));
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            surrounding[counter] = *get_tree(x+j,y+i,width,forest);
            printf("%d",surrounding[counter].status);
            counter++;
        }
        printf("\n");
    }
    free(surrounding);
}
void scan_settings(int* width, int* height, double* density) {
    do{
        printf("Please enter a width, height, and forest density (0.00 - 1):\n");
        scanf(" %d %d %lf", width, height, density);
    } while (!(*width <= MAX_WIDTH && *width > 0 && *height <= MAX_HEIGHT && *height > 0 && *density <= 1 && *density >= 0));
}

void user_drop_water(tree_t* forest, int x, int y, int width) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            get_tree(x+j,y+i,width,forest)->status = wet;
        }
    }
}

void user_dead_zone(tree_t* forest, int x, int y, int width, int size_of_dead_zone) {
    if (size_of_dead_zone + x < width || size_of_dead_zone - x >= 0){
        for (int j = -size_of_dead_zone; j <= size_of_dead_zone; j++) {
            get_tree(x+j,y-size_of_dead_zone,width,forest)->status = empty;
        }
        for (int j = -size_of_dead_zone; j <= size_of_dead_zone; j++) {
            get_tree(x+j,y+size_of_dead_zone,width,forest)->status = empty;
        }
        for (int j = -size_of_dead_zone; j <= size_of_dead_zone; j++) {
            get_tree(x-size_of_dead_zone,y+j,width, forest)->status = empty;
        }
        for (int j = -size_of_dead_zone; j <= size_of_dead_zone; j++) {
            get_tree(x+size_of_dead_zone,y+j,width, forest)->status = empty;
        }
    }
    else {
        printf("Input Error: dead_zone exceeds forest\n");
    }
}