#include "fire-sim.h"
#include "console.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <unistd.h>

#define BLUE BACKGROUND_BLUE
#define GREEN BACKGROUND_GREEN
#define RED BACKGROUND_RED
#define WHITE FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define GREY BACKGROUND_INTENSITY
#define BLACK 0
#define TREE_REP "  "

#define RATE_OF_BURN 0.2
#define STARTING_FIRE_STRENGTH 40
#define SPREAD_FIRE_STRENGTH 15
void make_rnd_forest(tree_t* forest, double density, int size) {
    for (int i = 0; i < size; i++) {
        if (rand() % 100 < density * 100){
            forest[i].status = 1;
            forest[i].fuel_left = 1;
            forest[i].fire_strength = 0;
            forest[i].humidity = 1;
        }
        else {
            forest[i].status = 0;
            forest[i].fuel_left = 0;
            forest[i].fire_strength = 0;
            forest[i].humidity = 0;
        }
    }
}
void print_forest(tree_t* forest, int height, int width) {
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            switch (get_tree(i,j,width,forest)->status) {
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
            printf(TREE_REP);
        }
        printf("\n");
    }
    printf("\n");
    //Sørger for at alt der printes efter forest, er af normale farver
    reset_color();
}
tree_t* get_tree(int x, int y, int width, tree_t* forest) {
    return &forest[width * y + x];
}

//Når start er true starter brænden på disse x y koordinater
void start_fire(tree_t* forest, int x, int y, int width) {
    tree_t *tree = get_tree(x, y, width, forest);
    tree->status = burning;
    tree->fire_strength = STARTING_FIRE_STRENGTH;
    //"Hvorfor fanden har du startet en brand, er du fuldstændig vanvittig?!"
}

int check_surrounding_fire_strength(tree_t* forest, int x, int y,int width, int height) {
    int total_fire_strength = 0;

    for (int i = -1; i <= 1; i++) {
        if (0 <= y + i && y + i < height) { //hvis y-værdien er indenfor arrayets y-akse
            for (int j = -1; j <= 1; j++) {
                if (0 <= x + j && x + j < width) { //hvis x-værdien er indenfor arrayets x-akse
                    tree_t* tree = get_tree(x + j, y + i, width, forest);
                    if (tree->status == burning) {
                        total_fire_strength += tree->fire_strength;
                    }
                }
            }
        }
    }
    return total_fire_strength;
}

double calculate_risk_of_burning(tree_t* forest, int x, int y,int width, int height) {
    double chance = 0.0;
    double fire_strength = check_surrounding_fire_strength(forest, x, y, width, height);

    chance += ceil(fire_strength / 5) * 10;

    chance += forest->humidity * -10;

    return chance;
}

void user_drop_water(tree_t* forest, int x, int y, int width) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            get_tree(x+j,y+i,width,forest)->status = wet;
            get_tree(x+j,y+i,width,forest)->fire_strength = 0;
            get_tree(x+j,y+i,width,forest)->humidity = 100;


        }
    }
}

void user_dead_zone(tree_t* forest, int x, int y, int width, int size_of_dead_zone) {
    if (size_of_dead_zone + x < width || size_of_dead_zone - x >= 0){
        for (int j = -size_of_dead_zone; j <= size_of_dead_zone; j++) {
            get_tree(x+j,y-size_of_dead_zone,width,forest)->status = empty;
        }
        for (int j = -size_of_dead_zone; j <= size_of_dead_zone; j++) {
            get_tree(x+j,y+size_of_dead_zone,width,forest)->status = empty;
        }
        for (int j = -size_of_dead_zone; j <= size_of_dead_zone; j++) {
            get_tree(x-size_of_dead_zone,y+j,width, forest)->status = empty;
        }
        for (int j = -size_of_dead_zone; j <= size_of_dead_zone; j++) {
            get_tree(x+size_of_dead_zone,y+j,width, forest)->status = empty;
        }
    }
    else {
        printf("Input Error: dead_zone exceeds forest\n");
    }
}

int get_trees_amount(tree_t* forest, int size, status_e target) {
    int counter = 0;
    // Loop igennem alle træer
    for (int i = 0; i < size; i++) {
        if (forest[i].status == target) {
            counter++;
        }
    }

    return counter;
}
void status_text(wind_t* wind, tree_t* forest, int size) {
    printf("Status:\n");
    print_wind(wind);

    int fresh_count = get_trees_amount(forest, size, fresh);
    int burnt_count = get_trees_amount(forest, size, burnt);

    printf("Fresh trees: %4d\n", fresh_count);
    printf("Burnt trees: %4d\n", burnt_count);
}


//Burndown: For hvert træ der brænder, mister den brændstof med RATE_OF_BURN.
void burndown(tree_t* forest, int height, int width) {
    for (int i = 0; i < height; i++ ) {
        for (int j = 0; j < width; j++) {
            //Hvis status er empty, skal den blive ved med at være det.
            if (get_tree(j, i, width, forest)->status == empty) {
                continue;
            }

            if (get_tree(j, i, width, forest)->status == burning) {
                get_tree(j, i, width, forest)->fuel_left -= RATE_OF_BURN;
            }
            //Hvis brændstof er 0, ændres status til burnt.
            if (get_tree(j, i, width, forest)->fuel_left <= 0) {
                get_tree(j, i, width, forest)->status = burnt;
                get_tree(j, i, width, forest)->fire_strength = 0;
            }
        }
    }
}



int sim_finished_check(tree_t* forest, int size)
{
    int counter = get_trees_amount(forest, size, burning);
    if (counter > 0) {
        return 1;
    }
    return 0;
}
void spread(tree_t* forest, int height, int width, int* trees_to_burn) {
    for (int i = 0; i < width*height; i++) {
        if (&trees_to_burn[i] == NULL || trees_to_burn[i] == -1) {
            continue;
        }
        forest[trees_to_burn[i]].status = burning;
        //Fire strength sættes til en startværdi, angivet som en macro.
        forest[trees_to_burn[i]].fire_strength = SPREAD_FIRE_STRENGTH;
    }
    //Frigør trees_to_burn
    free (trees_to_burn);
}

int* scan_forest_spread(tree_t* forest, int height, int width) {
    //Initialiserer array til at være på størrelse med skoven,
    //og counter, som stiger med 1 hver gang et nyt element indlæses i arrayet.
    int* trees_to_burn = malloc(sizeof(int) * height * width);
    int counter = 0;
    //For hvert træ i skoven:
    for (int i = 0; i < height; i++ ) {
        for (int j = 0; j < width; j++) {
            //Hvis et træ brænder, er brændt eller er tom, skal det selvfølgelig ikke brændes. Derfor
            if (get_tree(j, i, width, forest)->status == burning ||
                get_tree(j, i, width, forest)->status == empty ||
                get_tree(j, i, width, forest)->status == burnt) {
                trees_to_burn[counter] = -1;
                counter++;
                continue;
            }
            //Initialisér risiko for at branden starter som procent fra 0 til 100.
            double risk_of_burning = calculate_risk_of_burning( forest,j,i,width, height);
            //Hvis et tilfældigt tal mellem 0 og 99 er under risikoen, så brænder træet. Ellers ikke.
            if (rand() % 100 < risk_of_burning) {
                trees_to_burn[counter] = i * width + j;
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
void tick(tree_t* forest, int height, int width, wind_t* wind, int start_y) {
    //Initialiserer variable for at se om simulationen er færdig.
    int check;
    do {
        //Vi vil have at brændende træer mister brændstof, og at ilden spreder sig.
        burndown(forest, height, width);
        //Vi scanner for hvilke træer der skal brænde
        int* trees_to_burn = scan_forest_spread(forest, height, width);
        //Hvis dette array ikke er en NULL pointer, fortsætter vi
        if (trees_to_burn != NULL) {
            spread(forest, height, width, trees_to_burn);
        }
        /*Vi sætter cursoren i konsollen til at være dér hvor skoven starter,
         *hvorefter vi printer skoven og statusteksten ovenpå.
         */
        SetConsoleCursorPosition(hConsole, (COORD){0,start_y});
        print_forest(forest,height,width);
        status_text(wind, forest, width * height);

        //Vi checker om simulationen er færdig
        check = sim_finished_check(forest, height*width);
        //Vi beder computeren om at vente 0.1 sekunder (10^5 mikrosekunder) mellem hver iteration
        usleep(pow(10,5));

        //Check om løkken er færdig
    } while (check == 1);
    printf("Sim is finished!\n");
}




