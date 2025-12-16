#include "fire-sim.h"
#include "input.h"
#include "console.h"
#include <stdlib.h>
#include <time.h>
#include <output.c>

int main(void) {
    srand(time(NULL));
    int width, height;
    double density;
    scan_settings(&width, &height, &density);

    console_setup();

    //Initialiserer tilfældig vind
    vector_t wind = rnd_wind();

    //Initialiserer en tilfældig skov
    forest_t forest = make_rnd_forest(density, width, height, wind);

    //finder et tilfældigt sted i skoven at starte branden
    int fire_start_x, fire_start_y;
    do
    {
        fire_start_x = rand() % forest.width;
        fire_start_y = rand() % forest.height;

    } while (!(get_tree(forest, fire_start_x, fire_start_y)->status != empty));

    start_fire(forest, fire_start_x, fire_start_y);

    //kører simulationen
    int tickCounter = 0;
    fire_sim(forest, &tickCounter);


    //Skriver en log til output filen
    write_output(forest,tickCounter,width,height,density,wind);
    free(forest.trees);

    //pauser programmet så brugeren kan se simulationens resultat da programmet ellers bare ville lukke ned
    system("pause");
    return 0;
}
