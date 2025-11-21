#include "input.h"
#include <stddef.h>
#include <windows.h>

//global variable changeable by any .c file that includes this library.
int accept_user_input;


//function will be run as another thread so user can make inputs while sim is running
void* user_input_loop(int *x, int *y, int *command)
{
    //loop ends when code in main runs accept_user_input = 0
    while (accept_user_input)
    {
        //code loop for user input
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

    int no_input = 1;
    while (no_input)
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
    } //loop will run indefinetly until user gives a valid input

    // Restore input mode on exit.
    SetConsoleMode(hStdin, fdwSaveOldMode);
}

//code to be run when user clicks
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

//code to be run when user presses a key
//the virtual code of the keys can be found here
//https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
int KeyEventProc(KEY_EVENT_RECORD ker, int *command)
{
    if (ker.bKeyDown) //if key is pressed
    {
        if (ker.wVirtualKeyCode == 0x41) //if key is 'A'
        {
            //code to be run when the key 'A' is pressed

        }
        return 1;
    }
    return 0;
}



