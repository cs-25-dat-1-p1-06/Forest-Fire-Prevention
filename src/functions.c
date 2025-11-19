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


void chance(tree_t *surrounding) {
    tree_t center = surrounding[4];

    for (int i = 0; i < 9; i++) {
        if (i == 4) continue;
        tree_t *neighbor = &surrounding[i];
        if (neighbor->status != fresh) continue;

        int chance = 30;

        switch (center.fire_strength) {
            case 1: chance +=5; break;
            case 2: chance +=10; break;
            case 3: chance +=15; break;
            case 4: chance +=20; break;
            case 5: chance +=25; break;
        }
        switch (neighbor->humidity) {
            case 0: chance-=0; break;
            case 1: chance-=10; break;
            case 2: chance-=20; break;
            case 3: chance-=30; break;
            case 4: chance-=40; break;
            case 5: chance-=50; break;
        }

        int roll = rand() % 100;

        if (roll < chance) {
            neighbor->status = burning;
            printf("Tree %d caught fire. Chance = %d Roll = %d \n",i,chance,roll);
        } else {
            printf("Tree %d did NOT catch fire. Chance = %d Roll %d \n",i,chance,roll);
        }
    }
}














