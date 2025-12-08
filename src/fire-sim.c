#include "fire-sim.h"
#include "probability.h"
#include "console.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include "input.h"


#define BLUE BACKGROUND_BLUE
#define GREEN BACKGROUND_GREEN
#define RED BACKGROUND_RED
#define WHITE FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define GREY BACKGROUND_INTENSITY
#define BLACK 0
#define TREE_REP "  "




forest_t make_rnd_forest(double density, int width, int height, vector_t wind) {


    tree_t* trees = malloc(sizeof(tree_t) * width * height);
    forest_t rnd_forest = {trees, width, height, width * height, wind};
    for (int i = 0; i < width * height; i++) {
        if (random_chance(density)) {
            rnd_forest.trees[i].status = fresh;
            rnd_forest.trees[i].fuel_left = TREE_FUEL;
            rnd_forest.trees[i].heat = 0;
            rnd_forest.trees[i].humidity = 1;
        }
        else {
            rnd_forest.trees[i].status = empty;
            rnd_forest.trees[i].fuel_left = 0;
            rnd_forest.trees[i].heat = 0;
            rnd_forest.trees[i].humidity = 0;
        }
    }

    return rnd_forest;
}
void print_forest(forest_t forest, short start_y) {
    //Vi sætter cursoren i konsollen til at være dér hvor skoven starter, hvorefter vi printer skoven
    SetConsoleCursorPosition(hConsole, (COORD){0, start_y});

    for (int j = 0; j < forest.height; j++) {
        for (int i = 0; i < forest.width; i++) {
            print_tree(*get_tree(forest, i, j));
            //printf("%.1lf ",get_tree(forest,i,j)->fuel_left);
        }
        printf("\n");
    }
    printf("\n");
}

void print_tree(tree_t tree)
{
    switch (tree.status) {
        case empty:
            color_change(BLACK);
            break;
        case fresh:
            color_change(GREEN);
            break;
        case burning:
            color_change(RED);
            break;
        case burnt:
            color_change(GREY);
            break;
        case wet:
            color_change(BLUE);
            break;
    }
    //Printer et tomrum på størrelse med et træ.
    printf("%s",TREE_REP);
}

tree_t* get_tree(forest_t forest, int x, int y) {
    return &forest.trees[forest.width * y + x];
}

int check_bounds(forest_t forest, int x, int y)
{
    return 0 <= x && x < forest.width && 0 <= y && y < forest.height;
}

void change_tree(forest_t forest, status_e new_status, int x, int y)
{
    if (check_bounds(forest, x, y))
    {
        tree_t* tree_to_change = get_tree(forest, x, y);
        switch (new_status) {
            case empty:
                destroy_tree(tree_to_change);
                break;
            case fresh:
                create_tree(tree_to_change);
                break;
            case burning:
                burn_tree(tree_to_change);
                break;
            case burnt:
                finish_burn(tree_to_change);
                break;
            case wet:
                water_tree(tree_to_change);
                break;
        }
    }
}

void destroy_tree(tree_t* tree_to_destroy)
{
    if (tree_to_destroy->status != burning && tree_to_destroy->status != burnt)
    {
        tree_to_destroy->status = empty;
        tree_to_destroy->humidity = 0;
        tree_to_destroy->heat = 0;
        tree_to_destroy->fuel_left = 0;
    }
}

void create_tree(tree_t* tree_to_create)
{
    tree_to_create->status = fresh;
    tree_to_create->humidity = 1;
    tree_to_create->heat = 0;
    tree_to_create->fuel_left = TREE_FUEL;
}

void burn_tree(tree_t* tree_to_burn)
{
    if (tree_to_burn->status != empty && tree_to_burn->status != burnt)
    {
        tree_to_burn->status = burning;
        tree_to_burn->humidity = 0;
        tree_to_burn->heat = SPREAD_HEAT;
        tree_to_burn->fuel_left = TREE_FUEL;
    }
}

void finish_burn(tree_t* tree_to_burnt)
{
    if (tree_to_burnt->status == burning)
    {
        tree_to_burnt->status = burnt;
        tree_to_burnt->humidity = 0;
        tree_to_burnt->heat = 0;
        tree_to_burnt->fuel_left = 0;
    }
}
void water_tree(tree_t* tree_to_water)
{
    if (tree_to_water->status != burning && tree_to_water->status != burnt && tree_to_water->status != empty)
    {
        tree_to_water->status = wet;
        tree_to_water->humidity = WET_TREE_HUMIDITY;
        tree_to_water->heat = 0;
    }
}

//Når start er true starter brænden på disse x y koordinater
void start_fire(forest_t forest, int x, int y) {
    if (check_bounds(forest, x, y)) {
        tree_t* tree_to_burn = get_tree(forest, x, y);
        burn_tree(tree_to_burn);
        tree_to_burn->heat = STARTING_HEAT;
    }
    //"Hvorfor fanden har du startet en brand, er du fuldstændig vanvittig?!"
}

double calculate_fire_prob(forest_t forest, int x, int y) {
    double not_fire_prob = 1;

    for (int i = -(int)SPREAD_RANGE; i <= SPREAD_RANGE; i++) {
        for (int j = -(int)SPREAD_RANGE; j <= SPREAD_RANGE; j++) {
            if (check_bounds(forest, x + j, y + i)) { //hvis både x- og y-værdien er indenfor arrayet
                tree_t* tree = get_tree(forest, x + j, y + i);

                vector_t distance = new_vector(-j, i);
                if (tree->status == burning && distance.length <= SPREAD_RANGE) {
                    //Vi bestemmer heat ift. afstanden. Svagere jo længere væk træet er.
                    double heat_by_dist = heat_from_distance(*tree, distance.length);
                    not_fire_prob *= heat_prob(heat_by_dist);
                    not_fire_prob *= wind_prob(forest.wind, distance);
                }
            }
        }
    }
    return 1 - not_fire_prob;
}

void user_drop_water(forest_t forest, int size_of_splash, int x, int y) {
    for (int i = -size_of_splash; i <= size_of_splash; i++) {
        for (int j = -size_of_splash; j <= size_of_splash; j++) {
            change_tree(forest, wet, x + j, y + i);
        }
    }
}

void user_dead_zone(forest_t forest, int size_of_dead_zone, int x, int y) {
    for (int j = -size_of_dead_zone; j <= size_of_dead_zone; j++) {
        change_tree(forest, empty, j + x, size_of_dead_zone + y);
        change_tree(forest, empty, j + x, -size_of_dead_zone + y);
    }
    for (int i = -size_of_dead_zone; i <= size_of_dead_zone; i++) {
        change_tree(forest, empty, size_of_dead_zone + x, i + y);
        change_tree(forest, empty, -size_of_dead_zone + x, i + y);
    }
    //
    // if (size_of_dead_zone + x < forest.width && x - size_of_dead_zone >= 0){
    //     for (int j = -size_of_dead_zone; j <= size_of_dead_zone; j++) {
    //         get_tree(forest, x+j, y-size_of_dead_zone)->status = empty;
    //         get_tree(forest, x-size_of_dead_zone, y+j)->status = empty;
    //         get_tree(forest, x+j, y+size_of_dead_zone)->status = empty;
    //         get_tree(forest, x+size_of_dead_zone, y+j)->status = empty;
    //     }
    // }
    // //Hvis dead-zone kommer udenfor forest, skal det selvfølgelig ikke bløde over i de andre linjer.
    // /* Først hvis x - size_of_dead_zone er under 0, skal den venstre linje ikke inkluderes
    //  * De vandrette linjer skal kun printes hvis x + j er over 0. Men den højre linje skal printes uanset hvad.
    //  */
    // else if (x - size_of_dead_zone < 0) {
    //     for (int j = -size_of_dead_zone; j <= size_of_dead_zone; j++) {
    //
    //         if (x + j >= 0) {
    //             get_tree(forest, x+j, y-size_of_dead_zone)->status = empty;
    //             get_tree(forest, x+j, y+size_of_dead_zone)->status = empty;
    //         }
    //             get_tree(forest, x+size_of_dead_zone, y+j)->status = empty;
    //
    //     }
    // }
    // else if (x + size_of_dead_zone >= forest.width) {
    //     for (int j = -size_of_dead_zone; j <= size_of_dead_zone; j++) {
    //         if (x + j < forest.width) {
    //             get_tree(forest, x+j, y-size_of_dead_zone)->status = empty;
    //             get_tree(forest, x+j, y+size_of_dead_zone)->status = empty;
    //         }
    //             get_tree(forest, x-size_of_dead_zone, y+j)->status = empty;
    //
    //     }
    // }

}



int get_trees_amount(forest_t forest, status_e target) {
    int counter = 0;
    // Loop igennem alle træer
    for (int i = 0; i < forest.size; i++) {
        if (forest.trees[i].status == target) {
            counter++;
        }
    }
    return counter;
}


//Burndown: For hvert træ der brænder, mister den brændstof med RATE_OF_BURN.
void burndown(forest_t forest) {
    for (int i = 0; i < forest.height; i++ ) {
        for (int j = 0; j < forest.width; j++) {
            tree_t* tree = get_tree(forest, j, i);
            //Hvis status er empty, skal den blive ved med at være det.
            if (tree->status == empty) {
                continue;
            }

            if (tree->status == burning) {
                tree->fuel_left -= RATE_OF_BURN;
            }
            //Hvis brændstof er 0, ændres status til burnt.
            if (tree->fuel_left <= 0) {
                change_tree(forest, burnt, j, i);
            }
        }
    }
}


int sim_finished_check(forest_t forest)
{
    int counter = get_trees_amount(forest, burning);
    if (counter > 0) {
        return 0;
    }
    return 1;
}

void spread(forest_t forest, int* trees_to_burn) {
    if (trees_to_burn != NULL)
    {
        for (int i = 0; i < forest.size; i++) {
            if (&trees_to_burn[i] == NULL || trees_to_burn[i] == -1) {
                continue;
            }
            int x = trees_to_burn[i] % forest.width;
            int y = trees_to_burn[i] / forest.width;
            change_tree(forest, burning, x, y);
        }
    }
    //Frigør trees_to_burn
    free(trees_to_burn);
}

int* scan_forest_spread(forest_t forest) {
    //Initialiserer array til at være på størrelse med skoven,
    //og counter, som stiger med 1 hver gang et nyt element indlæses i arrayet.
    int* trees_to_burn = malloc(sizeof(int) * forest.size);
    int counter = 0;
    //For hvert træ i skoven:
    for (int i = 0; i < forest.height; i++ ) {
        for (int j = 0; j < forest.width; j++) {
            tree_t* current_tree = get_tree(forest, j, i);
            //Hvis et træ brænder, er brændt eller er tom, skal det selvfølgelig ikke brændes.
            switch (current_tree->status)
            {
                case fresh:
                    //Initialisér risiko for at branden starter som procent fra 0 til 100.
                    double risk_of_burning = calculate_fire_prob(forest, j, i);
                    //Hvis et tilfældigt tal mellem 0 og 99 er under risikoen, så brænder træet. Ellers ikke.
                    if (random_chance(risk_of_burning)) {
                        trees_to_burn[counter] = i * forest.width + j;
                    }
                    else {
                        trees_to_burn[counter] = -1;
                    }
                    break;
                default:
                trees_to_burn[counter] = -1;

                break;
            }
            counter++;
        }
    }
    return trees_to_burn;
}
//Laver en tick funktion der håndterer alt der skal ske når et tick foregår.
void tick(forest_t forest)
{
    //Vi vil have at brændende træer mister brændstof, og at ilden spreder sig.
    burndown(forest);
    //Vi scanner for hvilke træer der skal brænde
    int* trees_to_burn = scan_forest_spread(forest);
    //Hvis dette array ikke er en NULL pointer, fortsætter vi

    spread(forest, trees_to_burn);

}

void fire_sim(forest_t forest, int* tickCounter, short start_y) {
    pthread_mutex_t accept_user_input;
    pthread_mutex_init(&accept_user_input, NULL);
    pthread_mutex_lock(&accept_user_input);



    input_t user_input = {0, 0, start_y, 0, 1, forest, &accept_user_input};


    pthread_t input_thread;
    pthread_create(&input_thread, NULL, user_input_loop, &user_input);


    do {
        if (!user_input.paused)
        {
            tick(forest);
            (*tickCounter)++;
        }

        print_forest(forest, start_y);

        status_text(forest,*tickCounter);

        //Vi beder computeren om at vente 0.1 sekunder (10^5 mikrosekunder) mellem hver iteration
        usleep(pow(10,5));

        //Vi checker om simulationen er færdig
    } while (!sim_finished_check(forest));
    printf("Sim is finished!\n");
    
    pthread_mutex_unlock(&accept_user_input);
    pthread_join(input_thread, NULL);
}

void status_text(forest_t forest, int tickCount) {
    //Sørger for at status tekst, er af normale farver
    reset_color();
    printf("Status:\n");
    print_wind(forest.wind);

    int fresh_count = get_trees_amount(forest, fresh);
    int burnt_count = get_trees_amount(forest, burnt);

    printf("%11s: %4d\n", "Fresh trees", fresh_count);
    printf("%11s: %4d\n", "Burnt trees", burnt_count);
    printf("%11s: %4d\n", "Tick count", tickCount);
}

