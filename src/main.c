#include "functions.h"
#include "wind.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(void) {
    srand(time(NULL));
    console_setup();

    int width, height;
    double density;
    scan_settings(&width, &height, &density);
    tree_t* forest = malloc(width*height*sizeof(tree_t));
    make_rnd_forest(forest, density, width*height);
    print_forest(forest, height, width);
    system("pause");
    
    //Initiliasere vind
    struct Wind *wind = (struct Wind*) malloc(sizeof(struct Wind));

    //Tjekker om allokeringen var en success
    if (wind == NULL) {
        printf("Der er ikke plads i hukommelsen!");
    }
    start_wind(wind);
    
    free(forest);
    free(wind);
    
    return 0;
}


