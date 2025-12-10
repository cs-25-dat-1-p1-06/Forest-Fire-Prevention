#ifndef FOREST_FIRE_PREVENTION_INPUT_H
#define FOREST_FIRE_PREVENTION_INPUT_H
#include <pthread.h>

#include "windows.h"
#include "fire-sim.h"

//alle commands som brugen kan bruge, angivet med virtuelle koder til taster
//de virtuelle koder til tasterne kan findes her:
//https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
typedef enum {none = 0x00, pause = 0x20, forest_thinning = 0x46, drop_water = 0x57, dead_zone = 0x44} command_e;

//struct til input, indeholder de nødvendige værdiger for at gemme brugens input
typedef struct
{
    int x;
    int y;
    command_e command;
    short start_y;
    int paused;
    forest_t forest;
    pthread_mutex_t* accept_user_input;
} input_t;

/**
 * loop til brugerens input
 * @param args pointer til den data som skal sendes til threaden, helst en input_t struct
 * @return NULL når den er færdig
 */
void* user_input_loop(void* args);

/**
 * tager brugerens input fra mus eller tastatur
 * @param input struct til input
 * @param hStdin input buffer handle
 */
void user_input(input_t* input, HANDLE hStdin);

/**
 * assigner værdi til x og y baseret på hvor brugeren klikker i konsollen
 * @param mer muse event
 * @param x pointer til x værdien af der hvor brugeren klikker
 * @param y pointer til y værdien af der hvor brugeren klikker
 */
void MouseEventProc(MOUSE_EVENT_RECORD mer, int *x, int *y, int start_y);

/**
 * assigner værdi til kommand baseret på tastatur input
 * @param ker tastatur event
 * @param command pointer til den command som skal gives baseret på tastatur input
 */
void KeyEventProc(KEY_EVENT_RECORD ker, command_e *command);

/**
 * tillader brugeren at indtaste størrelse og tæthed af skoven
 * @param width pointer til den ønskede bredde af skoven
 * @param height pointer til den ønskede højde af skoven
 * @param density pointer til en sandsynlighedsværdi mellem 0 og 1 for om der er et træ i en celle
 */
void scan_settings(int* width, int* height, double* density);

#endif //FOREST_FIRE_PREVENTION_INPUT_H