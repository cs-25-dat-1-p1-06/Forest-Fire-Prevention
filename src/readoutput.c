#include "fire-sim.h"
#include "input.h"
#include "console.h"
#include <stdlib.h>
#include <time.h>
#include <output.c>

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


    int tickCounter = 0;
    fire_sim(forest, &tickCounter);


    //Output file
    write_output(forest,tickCounter,width,height,density);
    free(forest.trees);

    system("pause");
    return 0;
}
