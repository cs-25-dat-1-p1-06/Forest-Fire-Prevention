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
    tree_t* forest = malloc(width*height*sizeof(tree_t));
    make_rnd_forest(forest, density, width*height);
    print_forest(forest, height, width);
    system("pause");
    free(forest);

    return 0;
}


