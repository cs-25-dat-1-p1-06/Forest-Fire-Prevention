#include "fire-sim.h"
#include "input.h"
#include "console.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <output.c>




int main(void) {
    srand(time(NULL));

    int width, height;
    double density;
    scan_settings(&width, &height, &density);

    console_setup();

    //Initiliasere vind
    wind_t wind = rnd_wind();

    forest_t forest = make_rnd_forest(density, width, height, wind);


    int fire_start_x, fire_start_y;
    int tickCounter = 0;

    do
    {
        fire_start_x = rand() % forest.width;
        fire_start_y = rand() % forest.height;

    } while (!(get_tree(forest, fire_start_x, fire_start_y)->status != empty));

    start_fire(forest, fire_start_x, fire_start_y);
    CONSOLE_SCREEN_BUFFER_INFO start_buffer;
    GetConsoleScreenBufferInfo(hConsole, &start_buffer);
    COORD start_coord = start_buffer.dwCursorPosition;


    fire_sim(forest, start_coord.Y, &tickCounter);

    //Output file
    write_output(forest,tickCounter,width,height,density);
    free(forest.trees);

    system("pause");
    return 0;
}