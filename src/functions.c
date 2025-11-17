#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

#define MAX_WIDTH 500
#define MAX_HEIGHT 100

#define GREEN BACKGROUND_GREEN
#define RED BACKGROUND_RED
#define WHITE BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE
#define GREY BACKGROUND_INTENSITY
#define BLACK 0
#define TREE_REP "  "


void make_rnd_forest(tree_t* forest, double density, int size) {
    for (int i = 0; i < size; i++) {
        if (rand() % 100 < density * 100){
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
            switch (get_tree(i,j,width,forest).status) { //ændrer farven alt efter status på træet og så printes træet på linje 45
                case empty:
                    color_change(BLACK);
                    break;
                case fresh:
                    color_change(GREEN);
                    break;
                case burning:
                    color_change(RED);
                    break;
                case burnt:
                    color_change(GREY);
                    break;
            }
            printf(TREE_REP);
        }
        printf("\n");
    }
    color_change(BLACK);
}
tree_t get_tree(int x, int y, int width, tree_t* forest) {
    return forest[width * y + x];
}
//Når start er true starter brænden på disse x y kordinater
void start_brand(tree_t* forest, int x, int y, int width) {

    tree_t tree = get_tree(x, y, width, forest);
    tree.status = burning;
    //"Hvorfor fanden har du startet en brand, er du fuldstændig vanvittig"
}

// a function to return a prosent chance based on different factors

 int chance(int procent, int forest_thinning, int is_wet) {

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

void color_change(unsigned short color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, color);
}

void scan_settings(int* width, int* height, double* density) {
    do{
        printf("Please enter a width, height, and forest density (0.00 - 1):\n");
        scanf(" %d %d %lf", width, height, density);
    } while (!(*width <= MAX_WIDTH && *width > 0 && *height <= MAX_HEIGHT && *height > 0 && *density <= 1 && *density >= 0));

}

