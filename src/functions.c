#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

#define MAX_WIDTH 500
#define MAX_HEIGHT 100

#define GREEN BACKGROUND_GREEN
#define RED BACKGROUND_RED
#define WHITE FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define GREY BACKGROUND_INTENSITY
#define BLACK 0
#define TREE_REP "  "

#define RATE_OF_BURN 0.2

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
    printf("\n");
    color_change(BLACK);
    color_change(WHITE);
}
tree_t* get_tree(int x, int y, int width, tree_t* forest) {
    return &forest[width * y + x];
}
//Når start er true starter brænden på disse x y koordinater
void start_brand(tree_t* forest, int x, int y, int width) {
    tree_t *tree = get_tree(x, y, width, forest);
    tree->status = burning;
    //"Hvorfor fanden har du startet en brand, er du fuldstændig vanvittig"
}

// a function to return a prosent chance based on different factors


int check_surrounding_firestrength(tree_t* forest, int x, int y,int width, int height) {
    int total_firestrength = 0;
    for (int i = -1; i <= 1 && y + i < height; i++) {
        if (y + i > 0) {
            for (int j = -1; j <= 1 && x + j < width; j++) {
                if (x + j > 0) {
                    tree_t* tree = get_tree(x + j, y + i, width, forest);
                    if (tree->status == burning) {
                        total_firestrength += tree->fire_strength;
                    }
                }
            }
        }
    }
    return total_firestrength;
}

double calculate_risk_of_burning(tree_t* forest, int x, int y,int width, int height) {
    double chance=0.0;
    int fire_strength= check_surrounding_firestrength(forest, x, y, width, height);

    if (fire_strength >= 1 && fire_strength <= 5) {
        chance+= 10;
    }
    if (fire_strength >= 6 && fire_strength <= 10) {
        chance+= 20;
    }
    if (fire_strength >= 11 && fire_strength <= 15) {
        chance+= 30;
    }
    if (fire_strength >= 16 && fire_strength <= 20) {
        chance+= 40;
    }
    if (fire_strength >= 21 && fire_strength <= 25) {
        chance+= 50;
    }
    if (fire_strength >= 26 && fire_strength <= 30) {
        chance+= 60;
    }
    if (fire_strength >= 31 && fire_strength <= 35) {
        chance+= 70;
    }
    if (fire_strength >= 36 && fire_strength <= 40) {
        chance+= 80;
    }

    switch (forest->humidity) {
        case(1): chance-=10; break;
        case(2): chance-=20; break;
        case(3): chance-=30; break;
        case(4): chance-=40; break;
        case(5): chance-=50; break;
    }
    return chance;
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

void user_click_input(int *x, int *y)
{
    DWORD fdwSaveOldMode;
    HANDLE hStdin;

    DWORD cNumRead, fdwMode, i;
    INPUT_RECORD irInBuf[128];

    // Get the standard input handle.
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        exit(EXIT_FAILURE);

    // Save the current input mode, to be restored on exit.
    if (! GetConsoleMode(hStdin, &fdwSaveOldMode) )
        exit(EXIT_FAILURE);

    // Enable the window and mouse input events.
    // Disable quick edit mode because it interferes with receiving mouse inputs.
    fdwMode = (ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE;
    if (! SetConsoleMode(hStdin, fdwMode) )
        exit(EXIT_FAILURE);


    while (1)
    {
        // Wait for the events.
        if (! ReadConsoleInput(
                hStdin,      // input buffer handle
                irInBuf,     // buffer to read into
                128,         // size of read buffer
                &cNumRead) ) // number of records read
                    exit(EXIT_FAILURE);

        // Dispatch the events to the appropriate handler.
        for (i = 0; i < cNumRead; i++)
        {
            switch(irInBuf[i].EventType)
            {
            case MOUSE_EVENT: // mouse input
                if (MouseEventProc(irInBuf[i].Event.MouseEvent, x, y)) //loop will run indefinitely until user clicks somewhere
                {
                    // Restore input mode on exit.
                    SetConsoleMode(hStdin, fdwSaveOldMode);
                    return;
                }
            default:
                break;
            }
        }
    }
}

int MouseEventProc(MOUSE_EVENT_RECORD mer, int *x, int *y)
{
    if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
    {
        *x = mer.dwMousePosition.X / 2;
        *y = mer.dwMousePosition.Y;
        return 1;
    }
    return 0;
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
    ShowWindow(consoleWindow, SW_MAXIMIZE); // this mimics clicking on its maximize button
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
//Laver en tick funktion der håndterer alt der skal ske når et tick foregår.
void tick(tree_t* forest, int height, int width) {
    //Vi vil have at brændende træer mister brændstof, og at ilden spreder sig.
    burndown(forest, height, width);
    fire_spread(forest, height, width);
}
//Burndown: For hvert træ der brænder, mister den brændstof med RATE_OF_BURN.
void burndown(tree_t* forest, int height, int width) {
    for (int i = 0; i < height; i++ ) {
        for (int j = 0; j < width; j++) {
            if (get_tree(j, i, width, forest)->status == burning) {
                get_tree(j, i, width, forest)->fuel_left -= RATE_OF_BURN;
            }
            //Hvis brændstof er 0, ændres status til burnt.
            if (get_tree(j, i, width, forest)->fuel_left == 0) {
                get_tree(j, i, width, forest)->status = burnt;
            }
        }
    }
}
/*Fire_spread: Funktion der for hvert træ, checker alle træer der omringer det,
 *og fra en sandsynlighed får træet til at brænde*/
void fire_spread(tree_t* forest, int height, int width) {
    double risk_of_burning;
    tree_t* center_tree;
    int num_of_burning_trees;

    for (int i = 0; i < height; i++ ) {
        for (int j = 0; j < width; j++) {
            center_tree = get_tree(j, i, width, forest);
            num_of_burning_trees = check_surrounding_burning(forest, width, j, i);
            risk_of_burning = calculate_risk_of_burning();
        }
    }
}
//Function to check the amount of burning trees surrounding a given tree
int check_surrounding_burning(tree_t* forest, int width, int x, int y) {
    int burning_trees_counter = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (get_tree(j+x, i+y, width, forest) == NULL) {
                continue;
            }
            if (i == 0 && j == 0) {
                continue;
            }
            if (get_tree(j+x, i+y, width, forest)->status == burning) {
                burning_trees_counter++;
            }
        }
    }
    return burning_trees_counter;
}