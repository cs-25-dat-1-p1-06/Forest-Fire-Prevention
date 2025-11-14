//
// Created by oskar on 03-11-2025.
//

#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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