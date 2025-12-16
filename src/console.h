#ifndef FOREST_FIRE_PREVENTION_CONSOLE_H
#define FOREST_FIRE_PREVENTION_CONSOLE_H
#include <windows.h>
//dette variabel er sat til external så den kan bruges af alle filer som inkluderer console.h
extern HANDLE hConsole;


/**
 * får konsolens handle, gør markøren usynlig, tillader Console Virtual Terminal Sequences, og maksimerer vinduet
 */
void console_setup();

/**
 * nulstiller farverne i konsollen
 */
void reset_color();


#endif //FOREST_FIRE_PREVENTION_CONSOLE_H