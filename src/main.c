#include "fire-sim.h"
#include "input.h"
#include "console.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "probability.h"


int main(void) {
    srand(time(NULL));
    // printf("%f", chance_limiter(wind_prob()));
    int width, height;
    double density;
    scan_settings(&width, &height, &density);

    console_setup();

    //Initiliasere vind
    vector_t wind = rnd_wind();

    forest_t forest = make_rnd_forest(density, width, height, wind);


    int fire_start_x, fire_start_y;
    do
    {
        fire_start_x = rand() % forest.width;
        fire_start_y = rand() % forest.height;

    } while (!(get_tree(forest, fire_start_x, fire_start_y)->status != empty));

    start_fire(forest, fire_start_x, fire_start_y);
    CONSOLE_SCREEN_BUFFER_INFO start_buffer;
    GetConsoleScreenBufferInfo(hConsole, &start_buffer);
    COORD start_coord = start_buffer.dwCursorPosition;


    fire_sim(forest, start_coord.Y);



    free(forest.trees);

    system("pause");
    return 0;
}
