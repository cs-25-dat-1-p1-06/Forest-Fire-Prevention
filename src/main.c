#include "fire-sim.h"
#include "input.h"
#include "console.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>




int main(void) {
    srand(time(NULL));

    int width, height;
    double density;
    scan_settings(&width, &height, &density);

    console_setup();
    tree_t* forest = malloc(width*height*sizeof(tree_t));
    make_rnd_forest(forest, density, width*height);
    start_fire(forest,5,5,width);
    CONSOLE_SCREEN_BUFFER_INFO start_buffer;
    GetConsoleScreenBufferInfo(hConsole, &start_buffer);
    COORD start_coord = start_buffer.dwCursorPosition;
    print_forest(forest, height, width);

    //Initiliasere vind
    wind_t *wind = (wind_t*) malloc(sizeof(wind_t));

    //Tjekker om allokeringen var en success
    if (wind == NULL) {
        printf("Der er ikke plads i hukommelsen!");
        exit(EXIT_FAILURE);
    }
    start_wind(wind);
    status_text(wind, forest, width * height);
    tick(forest,height,width, wind, start_coord.Y);



    free(forest);
    free(wind);

    system("pause");
    return 0;
}


