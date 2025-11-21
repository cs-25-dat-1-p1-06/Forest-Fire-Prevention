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
    //Initialisere vind
    struct Wind *wind = (struct Wind*) malloc(sizeof(struct Wind));

    //Tjekker om allokeringen var en success
    if (wind == NULL) {
        printf("Der er ikke plads i hukommelsen!");
        exit(EXIT_FAILURE);
    }
    start_wind(wind);

    console_setup();
    tree_t* forest = malloc(width*height*sizeof(tree_t));
    make_rnd_forest(forest, density, width*height);
    print_forest(forest, height, width);
    system("pause");

    free(forest);
    free(wind);

    return 0;
}


