#include "input.h"
#include <stddef.h>
#include <stdio.h>
#include <windows.h>

#define MAX_WIDTH 500
#define MAX_HEIGHT 100

//global variabel som kan tilgås af andre .c filer, dette bruges når loopet skal stoppes af en anden .c fil
int accept_user_input;


//funktionen skal køres som en anden thread som kan modtage brugerens input imens simulationen kører
void* user_input_loop(int *x, int *y, int *command)
{
    accept_user_input = 1;
    //loop sluttter når kode i main kører accept_user_input = 0
    while (accept_user_input)
    {
        //kode loop til bruger input
        user_input(x, y, command);
    }
    return NULL;
}

//modificeret version af kode fra
//https://learn.microsoft.com/en-us/windows/console/reading-input-buffer-events
void user_input(int *x, int *y, int *command)
{
    DWORD fdwSaveOldMode;
    HANDLE hStdin;

    DWORD cNumRead, fdwMode, i;
    INPUT_RECORD irInBuf[128];

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

    int no_input = 1;
    while (no_input)
    {
        //venter på en event
        if (! ReadConsoleInput(
                hStdin,      // input buffer handle
                irInBuf,     // buffer to read into
                128,         // size of read buffer
                &cNumRead) ) // number of records read
                    exit(EXIT_FAILURE);

        //deler events ud til en passende funktion
        for (i = 0; i < cNumRead; i++)
        {
            switch(irInBuf[i].EventType)
            {
            case KEY_EVENT: //keyboard input
                if (KeyEventProc(irInBuf[i].Event.KeyEvent, command))
                    no_input = 0;

                break;
            case MOUSE_EVENT: // mouse input
                if (MouseEventProc(irInBuf[i].Event.MouseEvent, x, y))
                    no_input = 0;

                break;
            default:
                break;
            }
        }
    } //looper uendeligt indtil brugeren laver et gyldigt input

    //gendanner input mode inden slut
    SetConsoleMode(hStdin, fdwSaveOldMode);
}

//kode der skal køres når brugeren klikker et sted i konsollen
int MouseEventProc(MOUSE_EVENT_RECORD mer, int *x, int *y)
{
    if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) //hvis brugeren trykker på venstre muse knap
    {
        *x = mer.dwMousePosition.X / 2;
        *y = mer.dwMousePosition.Y;
        return 1;
    }
    return 0;
}

//kode der skal køres når brugeren trykker på en tast
//de virtuelle koder til tasterne kan findes her:
//https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
int KeyEventProc(KEY_EVENT_RECORD ker, int *command)
{
    if (ker.bKeyDown) //hvis en tast bliver trykket
    {
        if (ker.wVirtualKeyCode == 0x41) //hvis tasten er 'A'
        {
            //kode der skal køres når 'A' bliver trykket

        }
        return 1;
    }
    return 0;
}

void scan_settings(int* width, int* height, double* density) {
    do {
        printf("Please enter a width, height, and forest density (0.00 - 1):\n");
        scanf(" %d %d %lf", width, height, density);
        fseek(stdin,0,SEEK_END); //tømmer input bufferen
    } while (!(*width <= MAX_WIDTH && *width > 0 && *height <= MAX_HEIGHT && *height > 0 && *density <= 1 && *density >= 0));
}



