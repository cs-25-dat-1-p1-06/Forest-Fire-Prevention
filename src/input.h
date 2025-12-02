#ifndef FOREST_FIRE_PREVENTION_INPUT_H
#define FOREST_FIRE_PREVENTION_INPUT_H
#include "windows.h"
#include "fire-sim.h"

//gør dette variabel synlig for de filer der inkluderer denne header, så der kan tændes og slukkes for brugerens input
extern int accept_user_input;


//every possible user command
typedef enum {none = 0x00, pause = 0x20, forest_thinning = 0x46} command_e;

//example struct in case it is useable
typedef struct
{
    int x;
    int y;
    command_e command;
    int paused;
    tree_t* forest;
    int width;
    int height;
    int start_y;
} input_t;

/**
 * loop til brugerens input
 * @param x pointer til x værdien af der hvor brugeren klikker
 * @param y pointer til y værdien af der hvor brugeren klikker
 * @param command pointer til den command som skal gives baseret på tastatur input
 * @return NULL når den er færdig
 */
void* user_input_loop(void* args);

/**
 * tager brugerens input fra mus eller tastatur
 * @param x pointer til x værdien af der hvor brugeren klikker
 * @param y pointer til y værdien af der hvor brugeren klikker
 * @param command pointer til den command som skal gives baseret på tastatur input
 */
void user_input(int *x, int *y, command_e *command);
/**
 * tjekker hvor brugeren klikker
 * @param mer muse event
 * @param x pointer til x værdien af der hvor brugeren klikker
 * @param y pointer til y værdien af der hvor brugeren klikker
 * @return 0 hvis brugeren ikke klikker (uønskede input). 1 hvis brugeren klikker et sted i konsollen
 */
int MouseEventProc(MOUSE_EVENT_RECORD mer, int *x, int *y);

/**
 * tjekker hvilken tast der bliver trykket
 * @param ker tastatur event
 * @param command pointer til den command som skal gives baseret på tastatur input
 * @return 0 hvis der ikke blev trykket på en tast (uønskede input). 1 hvis der blev trykket på en tast
 */
int KeyEventProc(KEY_EVENT_RECORD ker, command_e *command);

/**
 * tillader brugeren at indtaste størrelse og tæthed af skoven
 * @param width pointer til den ønskede bredde af skoven
 * @param height pointer til den ønskede højde af skoven
 * @param density pointer til en sandsynlighedsværdi mellem 0 og 1 for om der er et træ i en celle
 */
void scan_settings(int* width, int* height, double* density);

#endif //FOREST_FIRE_PREVENTION_INPUT_H