#include "console.h"
HANDLE hConsole;


void color_change(unsigned short color)
{
    SetConsoleTextAttribute(hConsole, color);
}

void console_setup()
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    //hides the cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = 0; // set the cursor visibility
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    //maximizes window
    HWND consoleWindow = GetConsoleWindow(); // This gets the value Windows uses to identify your output window
    ShowWindow(consoleWindow, SW_MAXIMIZE); // this mimics clicking on its maximize button
}

void reset_color() {
    color_change(0);
    color_change(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

}