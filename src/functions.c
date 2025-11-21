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
            switch (get_tree(i,j,width,forest)->status) {
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
                case wet:
                    printf("4");
                    break;
            }
            printf(TREE_REP);
        }
        printf("\n");
    }
    color_change(BLACK);
}
tree_t* get_tree(int x, int y, int width, tree_t* forest) {
    return &forest[width * y + x];
}
//Når start er true starter brænden på disse x y kordinater
void start_brand(tree_t* forest, int x, int y, int width) {
    tree_t *tree = get_tree(x, y, width, forest);
    tree->status = burning;
    //"Hvorfor fanden har du startet en brand, er du fuldstændig vanvittig"
}

// a function to return a prosent chance based on different factors


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

void user_drop_water(tree_t* forest, int x, int y, int width) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            get_tree(x+j,y+i,width,forest)->status = wet;
        }
    }
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

void console_setup()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    //hides the cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = 0; // set the cursor visibility
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    //maximizes window
    HWND consoleWindow = GetConsoleWindow(); // This gets the value Windows uses to identify your output window
    ShowWindow(consoleWindow, SW_MAXIMIZE); // this mimics clicking on its' maximize button
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

