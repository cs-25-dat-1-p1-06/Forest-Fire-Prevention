#include "fire-sim.h"
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

#define RATE_OF_BURN 0.2
#define STARTING_HEAT 30
#define STARTING_TREE_FUEL 1.0
#define SPREAD_HEAT 15
#define SPREAD_RANGE 2
#define HEAT_FACTOR 0.1

forest_t make_rnd_forest(double density, int width, int height, wind_t wind) {
    tree_t* trees = malloc(sizeof(tree_t) * width * height);
    forest_t rnd_forest = {trees, width, height, width * height, wind};
    for (int i = 0; i < width * height; i++) {
        if (rand() % 100 < density * 100){
            rnd_forest.trees[i].status = fresh;
            rnd_forest.trees[i].fuel_left = STARTING_TREE_FUEL;
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
void print_forest(forest_t forest, int start_y) {
    //Vi sætter cursoren i konsollen til at være dér hvor skoven starter, hvorefter vi printer skoven
    SetConsoleCursorPosition(hConsole, (COORD){0,start_y});

    for (int j = 0; j < forest.height; j++) {
        for (int i = 0; i < forest.width; i++) {
            switch (get_tree(forest, i, j)->status) {
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
            //printf("%.1lf ",get_tree(forest,i,j)->fuel_left);
        }
        printf("\n");
    }
    printf("\n");
    //Sørger for at alt der printes efter forest, er af normale farver
    reset_color();
}
tree_t* get_tree(forest_t forest, int x, int y) {
    return &forest.trees[forest.width * y + x];
}

//Når start er true starter brænden på disse x y koordinater
void start_fire(forest_t forest, int x, int y) {
    tree_t *tree = get_tree(forest, x, y);
    tree->status = burning;
    tree->heat = STARTING_HEAT;
    tree->fuel_left = STARTING_TREE_FUEL;
    //"Hvorfor fanden har du startet en brand, er du fuldstændig vanvittig?!"
}

double calculate_fire_prob(forest_t forest, int x, int y) {
    double not_fire_prob = 1;
    double wind_factor = forest.wind.speed;

    double corner_distance = sqrt(pow(SPREAD_RANGE, 2) + pow(SPREAD_RANGE, 2));
    for (int i = -SPREAD_RANGE; i <= SPREAD_RANGE; i++) {
        if (0 <= y + i && y + i < forest.height) { //hvis y-værdien er indenfor arrayets y-akse
            for (int j = -SPREAD_RANGE; j <= SPREAD_RANGE; j++) {
                if (0 <= x + j && x + j < forest.width) { //hvis x-værdien er indenfor arrayets x-akse
                    tree_t* tree = get_tree(forest, x + j, y + i);

                    double distance = sqrt(pow(i, 2) + pow(j, 2));
                    if (distance >= corner_distance)
                        continue;

                    if (tree->status == burning) {
                        //Vi bestemmer heat ift. afstanden. Svagere jo længere væk træet er.
                        double distance = distance_given_coord(i,j);
                        double heat_by_dist = heat_from_distance(*tree, distance);
                        not_fire_prob *= heat_prob(heat_by_dist);



                        // switch (forest.wind.direction) { // Vi tager højde for vind. Ved at tilføje en faktor ift. retning
                        //     case NORTH:
                        //         if (i > 0) {
                        //             not_fire_prob *= 1 - 1 / wind_factor;
                        //         }
                        //         else if (i < 0) {
                        //             not_fire_prob *= 1 + 0.01 * wind_factor;
                        //         }
                        //         break;
                        //     case EAST:
                        //         if (j < 0) {
                        //             not_fire_prob *= 1 - 1 / wind_factor;
                        //         }
                        //         else if (j > 0) {
                        //             not_fire_prob *= 1 + 0.01 * wind_factor;
                        //         }
                        //         break;
                        //     case SOUTH:
                        //         if (i < 0) {
                        //             not_fire_prob *= 1 - 1 / wind_factor;
                        //         }
                        //         else if (i > 0) {
                        //             not_fire_prob *= 1 + 0.01 * wind_factor;
                        //         }
                        //         break;
                        //     case WEST:
                        //         if (j > 0) {
                        //             not_fire_prob *= 1 - 1 / wind_factor;
                        //         }
                        //         else if (j < 0) {
                        //             not_fire_prob *= 1 + 0.01 * wind_factor;
                        //         }
                        //         break;
                        // }
                    }
                }
            }
        }
    }
    return 1 - not_fire_prob;
}



void user_drop_water(forest_t forest, int x, int y) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            tree_t* tree_to_water = get_tree(forest, x+j,y+i);

            tree_to_water->status = wet;
            tree_to_water->heat = 0;
            tree_to_water->humidity = 100;
        }
    }
}

void user_dead_zone(forest_t forest, int x, int y, int width, int size_of_dead_zone) {
    if (size_of_dead_zone + x < width || size_of_dead_zone - x >= 0){
        for (int j = -size_of_dead_zone; j <= size_of_dead_zone; j++) {
            get_tree(forest, x+j, y-size_of_dead_zone)->status = empty;
            get_tree(forest, x-size_of_dead_zone, y+j)->status = empty;
            get_tree(forest, x+j, y+size_of_dead_zone)->status = empty;
            get_tree(forest, x+size_of_dead_zone, y+j)->status = empty;
        }
    }
    else {
        printf("Input Error: dead_zone exceeds forest\n");
    }
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

void status_text(forest_t forest, int tickCount) {
    printf("Status:\n");
    print_wind(&forest.wind);

    int fresh_count = get_trees_amount(forest, fresh);
    int burnt_count = get_trees_amount(forest, burnt);

    printf("Fresh trees: %4d\n", fresh_count);
    printf("Burnt trees: %4d\n", burnt_count);
    printf("Tick count: %4d\n", tickCount);
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
                tree->status = burnt;
                tree->heat = 0;
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
    for (int i = 0; i < forest.size; i++) {
        if (&trees_to_burn[i] == NULL || trees_to_burn[i] == -1) {
            continue;
        }
        forest.trees[trees_to_burn[i]].status = burning;
        //Heat sættes til en startværdi, angivet som en macro.
        forest.trees[trees_to_burn[i]].heat = SPREAD_HEAT;
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
            case burning:
            case empty:
            case burnt:
                trees_to_burn[counter] = -1;
                counter++;
                continue;
            default: ;
            }

            //Initialisér risiko for at branden starter som procent fra 0 til 100.
            double risk_of_burning = calculate_fire_prob(forest, j, i);
            //Hvis et tilfældigt tal mellem 0 og 99 er under risikoen, så brænder træet. Ellers ikke.
            double random_chance = (double)(rand() % 101) / 100 ;
            if (random_chance < risk_of_burning) {
                trees_to_burn[counter] = i * forest.width + j;
                counter++;
            }
            else {
                trees_to_burn[counter] = -1;
                counter++;
            }
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
    if (trees_to_burn != NULL) {
        spread(forest, trees_to_burn);
    }
}

void fire_sim(forest_t forest, int start_y, int* tickCounter) {
    input_t user_input = {0, -1, 0, 1, forest, start_y};

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
    pthread_cancel(input_thread);

    printf("Sim is finished!\n");
}

void destroy_tree(forest_t forest, int x, int y, int start_y)
{
    if (x < forest.width && 0 <= y - start_y && y - start_y < forest.height)
    {
        tree_t* tree_to_destroy = get_tree(forest, x, y - start_y);
        if (tree_to_destroy->status != burning && tree_to_destroy->status != burnt)
        {
            tree_to_destroy->status = empty;
        }
    }
}
double heat_from_distance(tree_t tree, double distance){
    return heat_by_fuel_left(tree) / (pow(distance, 2) * 2);
}
double heat_prob(double heat) {
    return 1 - heat;
}
double wind_prob(wind_t wind) {
}
double distance_given_coord(int a, int b) {
    return sqrt(pow(a, 2) + pow(b, 2));
}
double heat_by_fuel_left(tree_t tree) {
    if (tree.status != burning) return 0;

    double my = STARTING_TREE_FUEL/2;
    double x = tree.fuel_left;
    double sigma = 0.2;
    double heat = 1;
    heat *= 1 / sqrt(2 * M_PI * pow(sigma,2));
    heat *= pow(M_E,- pow(x - my,2) / 2 * pow(sigma,2));
    heat *= HEAT_FACTOR;
    return heat;
}


