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
    // Disable quick edit mode because it interfers with receiving mouse inputs.
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
                if (MouseEventProc(irInBuf[i].Event.MouseEvent, x, y)) //loop will run indefinetly until user clicks somewhere
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
int get_trees_amount(tree_t* forest, int height, int width,const char* status_str) {
    int counter=0;
    status_e target;

    // Oversæt tekst til enum
    if (strcmp(status_str, "empty") == 0) target = empty;
    else if (strcmp(status_str, "fresh") == 0) target = fresh;
    else if (strcmp(status_str, "burning") == 0) target = burning;
    else if (strcmp(status_str, "burnt") == 0) target = burnt;
    else if (strcmp(status_str, "wet") == 0) target = wet;
    else return -1; // Ukendt status

    // Loop igennem alle træer
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            tree_t* t = get_tree(i, j, width, forest);
            if (t->status == target) {
                counter++;
            }
        }
    }

    return counter;
}
void status_text(wind_t* wind, tree_t* forest, int height, int width) {
    printf("Status:\n");
    print_wind(wind);

    int fresh_count = get_trees_amount(forest, height, width, "fresh");
    int burnt_count = get_trees_amount(forest, height, width, "burnt");

    printf("Fresh trees: %d\n", fresh_count);
    printf("Burnt trees: %d\n", burnt_count);
}
