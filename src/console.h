#ifndef FOREST_FIRE_PREVENTION_CONSOLE_H
#define FOREST_FIRE_PREVENTION_CONSOLE_H
#include <windows.h>
extern HANDLE hConsole;


/**
 * ændrer endten baggrundsfarven eller tekst farven baseret på input
 * @param color den farve der skal ændres til
 */
void color_change(unsigned short color);

/**
 * får konsolens handle, gør markøren usynlig, , og maksimerer vinduet
 */
void console_setup();
void reset_color();


#endif //FOREST_FIRE_PREVENTION_CONSOLE_H