#include "console.h"

#include <stdio.h>
HANDLE hConsole;

void console_setup()
{
    //får standard handle til konsollens output
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    //gær cursoren usynlig
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = 0; //gør markøren usynlig
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    //maksimerer vinduet
    HWND consoleWindow = GetConsoleWindow(); //tager værdien som Windows bruger til at identificere output vinduet (konsollen)
    ShowWindow(consoleWindow, SW_MAXIMIZE); //maksimerer output vinduet

    //tillader brugen af Console Virtual Terminal Sequences i tekst strenge printet til konsollen
    DWORD current_fdwMode;
    GetConsoleMode(hConsole, &current_fdwMode);
    current_fdwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, current_fdwMode);
}
void reset_color()
{
    //printer en Console Virtual Terminal Sequences som nulstiller farverne i konsollen
    printf("\x1b[0m");
}