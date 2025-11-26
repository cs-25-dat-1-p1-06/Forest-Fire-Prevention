#ifndef FOREST_FIRE_PREVENTION_CONSOLE_H
#define FOREST_FIRE_PREVENTION_CONSOLE_H
#include <windows.h>
extern HANDLE hConsole;

void color_change(unsigned short color);
void console_setup();
void reset_color();


#endif //FOREST_FIRE_PREVENTION_CONSOLE_H