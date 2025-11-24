#ifndef FOREST_FIRE_PREVENTION_INPUT_H
#define FOREST_FIRE_PREVENTION_INPUT_H
#include "windows.h"

//gør dette variabel synlig for de filer der inkluderer denne header, så der kan tændes og slukkes for brugerens input
extern int accept_user_input;


//every possible user command
typedef enum {water} command_e;

//example struct in case it is useable
typedef struct
{
    int x;
    int y;
    command_e command;
} input_t;


void* user_input_loop(int *x, int *y, int *command);
void user_input(int *x, int *y, int *command);
int MouseEventProc(MOUSE_EVENT_RECORD mer, int *x, int *y);
int KeyEventProc(KEY_EVENT_RECORD ker, int *command);

void scan_settings(int* width, int* height, double* density);

#endif //FOREST_FIRE_PREVENTION_INPUT_H