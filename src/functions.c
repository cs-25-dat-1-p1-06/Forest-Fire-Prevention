//
// Created by oskar on 03-11-2025.
//

#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
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
void scan_settings(int* width, int* height, double* density) {
    do{
        printf("Please enter a width, height, and forest density (0.00 - 1):\n");
        scanf(" %d %d %lf", width, height, density);
    } while (!(*width <= MAX_WIDTH && *width > 0 && *height <= MAX_HEIGHT && *height > 0 && *density <= 1 && *density >= 0));

}

