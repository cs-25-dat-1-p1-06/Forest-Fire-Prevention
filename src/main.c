#include "functions.h"
#include "wind.h"
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
    print_forest(forest, height, width);

    //Initiliasere vind
    wind_t *wind = (wind_t*) malloc(sizeof(wind_t));

    //Tjekker om allokeringen var en success
    if (wind == NULL) {
        printf("Der er ikke plads i hukommelsen!");
        exit(EXIT_FAILURE);
    }
    start_wind(wind);
    free(forest);
    free(wind);

    system("pause");
    return 0;
}


