#include "console.h"

void color_change(unsigned short color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, color);
}

void console_setup()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    //hides the cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = 0; //gør markøren usynlig
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    //maximizes window
    HWND consoleWindow = GetConsoleWindow(); //tager værdien som Windows bruger til at identificere output vinduet (konsollen)
    ShowWindow(consoleWindow, SW_MAXIMIZE); //maksimerer output vinduet

    SetConsoleOutputCP(CP_UTF8);
}