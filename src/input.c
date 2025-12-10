#include "input.h"
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <windows.h>
#include "console.h"

#define MAX_WIDTH 500
#define MAX_HEIGHT 100



//funktionen skal køres som en anden thread som kan modtage brugerens input imens simulationen kører
void* user_input_loop(void* args)
{
    input_t* input = args;
    command_e old_input;

    DWORD fdwSaveOldMode, fdwMode;
    HANDLE hStdin;

    //får standard input handle
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        exit(EXIT_FAILURE);

    //gemmer den nuværende input mode så den kan gendannes inden slut
    if (! GetConsoleMode(hStdin, &fdwSaveOldMode) )
        exit(EXIT_FAILURE);

    //aktiverer vindue og muse input events
    //deaktiverer quick edit mode da dette skaber problemer med at modtage muse inputs.
    fdwMode = (ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE;
    if (! SetConsoleMode(hStdin, fdwMode) )
        exit(EXIT_FAILURE);


    //hvis accept_user_input er en ulåst mutex stoppes loopet
    while (pthread_mutex_trylock(input->accept_user_input) == EBUSY)
    {
        old_input = input->command;
        //kode loop til bruger input
        user_input(input, hStdin);

        switch (input->command)
        {
        case pause:
            input->paused = !input->paused; //omvender paused så 0 bliver til 1 og 1 bliver til 0
            input->command = old_input;
            break;
        case forest_thinning:
            change_tree_at_coords(input->forest, empty, input->x, input->y);
            break;
        case drop_water:
            user_drop_water(input->forest, USER_SPLASH_ZONE_SIZE, input->x, input->y);
            break;
        case dead_zone:
            user_dead_zone(input->forest, USER_DEAD_ZONE_SIZE, input->x, input->y);
            break;
        }
        input->y = -MAX_HEIGHT;
    }
    //gendanner input mode inden slut
    SetConsoleMode(hStdin, fdwSaveOldMode);
    return NULL;
}

//modificeret version af kode fra
//https://learn.microsoft.com/en-us/windows/console/reading-input-buffer-events
void user_input(input_t* input, HANDLE hStdin)
{
    DWORD cNumRead;
    INPUT_RECORD irInBuf[128];

    if (! ReadConsoleInput(
            hStdin,      // input buffer handle
            irInBuf,     // buffer der skal læses ind i
            128,         // størrelsen på read buffer
            &cNumRead) ) // antallet af input der bliver læst
                exit(EXIT_FAILURE);

    //deler events ud til en passende funktion
    for (int i = 0; i < cNumRead; i++)
    {
        switch(irInBuf[i].EventType)
        {
            case KEY_EVENT: //keyboard input
                KeyEventProc(irInBuf[i].Event.KeyEvent, &input->command);
                break;
            case MOUSE_EVENT: // mouse input
                MouseEventProc(irInBuf[i].Event.MouseEvent, &input->x, &input->y, input->start_y);
                break;
        }
    }
}

//kode der skal køres når brugeren klikker et sted i konsollen
void MouseEventProc(MOUSE_EVENT_RECORD mer, int *x, int *y, short start_y)
{
    if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) //hvis brugeren trykker på venstre muse knap
    {
        *x = mer.dwMousePosition.X / 2;
        *y = mer.dwMousePosition.Y - start_y;
    }
}

//kode der skal køres når brugeren trykker på en tast
void KeyEventProc(KEY_EVENT_RECORD ker, command_e *command)
{
    if (ker.bKeyDown) //hvis en tast bliver trykket
    {
        *command = ker.wVirtualKeyCode;
    }
}

void scan_settings(int* width, int* height, double* density) {
    printf("To access different modes use the following keys:\n");
    printf("Forest Thinning: 'F'\n");
    printf("Water Drop: 'W'\n");
    printf("Dead Zone (size of 3): 'D'\n");
    printf("Pause: 'Space'\n");
    do {
        printf("Please enter a width, height, and forest density (0.00 - 1):\n");
        scanf(" %d %d %lf", width, height, density);
        fseek(stdin,0,SEEK_END); //tømmer input bufferen
    } while (!(*width <= MAX_WIDTH && *width > 0 && *height <= MAX_HEIGHT && *height > 0 && *density <= 1 && *density >= 0));
}



