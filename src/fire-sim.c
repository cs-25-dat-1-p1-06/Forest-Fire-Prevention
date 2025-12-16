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

//farver angivet i rgb
#define BLUE "0;0;200"
#define GREEN "0;200;0"
#define RED "200;0;0"
#define GREY "100;100;100"
#define BLACK "0;0;0"
#define TREE_REP "  "
#define MAX_SIZE_OF_COLOR 18 //længden på den længste SGR farve kode streng: "48;2;xxx;xxx;xxxm\0"




forest_t make_rnd_forest(double density, int width, int height, vector_t wind) {

    tree_t* trees = malloc(sizeof(tree_t) * width * height);
    forest_t rnd_forest = {trees, width, height, width * height, wind};
    for (int i = 0; i < width * height; i++) {
        if (random_chance(density)) {
            change_tree(&rnd_forest.trees[i], fresh);
        }
        else {
            change_tree(&rnd_forest.trees[i], empty);
        }
    }

    return rnd_forest;
}
void print_forest(forest_t forest, short start_y) {
    //Vi sætter cursoren i konsollen til at være dér hvor skoven starter, hvorefter vi printer skoven
    SetConsoleCursorPosition(hConsole, (COORD){0, start_y});
    unsigned long long buffer_size = sizeof(char) * forest.width * forest.height * (MAX_SIZE_OF_COLOR + strlen(TREE_REP)) + 1 * forest.height;
    /*
     * ændrer buffer mode til _IOFBF hvilket betyder at der skrives til konsollen i blokke af tekst på størrelse af bufferen fra bufferen
     * ændrer størrelsen på bufferen til at være den maksimale mulige antal karakterer som skal være inkluderet
    */
    setvbuf(stdout, NULL, _IOFBF, buffer_size);

    for (int j = 0; j < forest.height; j++) {
        for (int i = 0; i < forest.width; i++) {
            print_tree(*get_tree(forest, i, j));
        }
        printf("\n");
    }
    //sætter buffer mode tilbage til _IONBF så tekst skrives med det samme og sætter buffer size til BUFSIZ som er standard størrelsen
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);
    printf("\n");
}

void print_tree(tree_t tree)
{ //Vi starter med at initialisere color som en string, med starten på en farvekode.
    char color[MAX_SIZE_OF_COLOR] = "\x1b[48;2;";
    //Alt efter træets status tilføjer vi så resten af farvekoden, vha. strcat funktionen og vores farve-macroer.
    switch (tree.status) {
        case empty:
            strcat(color, BLACK);
            break;
        case fresh:
            strcat(color, GREEN);
            break;
        case burning:
            strcat(color, RED);
            break;
        case burnt:
            strcat(color, GREY);
            break;
        case wet:
            strcat(color, BLUE);
            break;
    }
    //Afsluttes med m
    strcat(color, "m");
    //Så printes farven, samt TREE_REP tomrummet, så cursoren bevæger sig som den skal.
    printf("%s%s", color, TREE_REP);
}

tree_t* get_tree(forest_t forest, int x, int y) {
    //Da y er højden, skal det ganges med bredden og plusses med x for at få det korrekte index
    return &forest.trees[forest.width * y + x];
}

int check_bounds(forest_t forest, int x, int y)
{
    //Hvis x og y ligger mellem 0 og henholdvis width og height, ligger de koordinater indenfor skovens grænser.
    return 0 <= x && x < forest.width && 0 <= y && y < forest.height;
}

void change_tree(tree_t* tree_to_change, status_e new_status) {
    //switch statement anvendes til at kalde forskellige funktionener ift. forskellig status man vil opnå.
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

void change_tree_at_coords(forest_t forest, status_e new_status, int x, int y) {
    //Hvis koordinaterne er indenfor bounds
    if (check_bounds(forest, x, y)) {
        //Vi henter træet
        tree_t* tree_to_change = get_tree(forest, x, y);
        //Vi kalder chance_tree med den status vi ønsker.
        change_tree(tree_to_change, new_status);
    }
}

void destroy_tree(tree_t* tree_to_destroy)
{
    //Hvis et træ brænder kan vi ikke fælde det, hvis det er brændt ville det ikke stoppe branden at fælde det.
    if (tree_to_destroy->status != burning && tree_to_destroy->status != burnt)
    {
        //Vi ændrer træets værdier til 0, og sætter status til empty.
        tree_to_destroy->status = empty;
        tree_to_destroy->humidity = 0;
        tree_to_destroy->heat = 0;
        tree_to_destroy->fuel_left = 0;
    }
}

void create_tree(tree_t* tree_to_create)
{
    //Vi opretter et træ med skovens fugtighed og brændstof, som alle andre træer i skoven.
    tree_to_create->status = fresh;
    tree_to_create->humidity = STARTING_HUMIDITY;
    tree_to_create->heat = 0;
    tree_to_create->fuel_left = TREE_FUEL;
}

void burn_tree(tree_t* tree_to_burn)
{
    //Hvis træet findes og ikke allerede er brændt.
    if (tree_to_burn->status != empty && tree_to_burn->status != burnt)
    {
        //Træet ændres til at have brændstof tilbage, dens fugtighed forsvinder, og varmen findes via. heat_by_fuel.
        tree_to_burn->status = burning;
        tree_to_burn->humidity = 0;
        tree_to_burn->fuel_left = TREE_FUEL;
        heat_by_fuel_left(tree_to_burn);
    }
}

void finish_burn(tree_t* tree_to_burnt)
{
    //Træet kan kun brænde ud hvis det brænder.
    if (tree_to_burnt->status == burning)
    {
        //Træet sættes til at være brændt ud, og dets værdier til 0.
        tree_to_burnt->status = burnt;
        tree_to_burnt->humidity = 0;
        tree_to_burnt->heat = 0;
        tree_to_burnt->fuel_left = 0;
    }
}
void water_tree(tree_t* tree_to_water)
{
    /* Hvis træet brænder, antages det, at vandet ville fordampe, og ikke påvirke træet.
     * Et brændt træ, eller et der ikke findes, ville ikke have gavn af vandet.
     */
    if (tree_to_water->status != burning && tree_to_water->status != burnt && tree_to_water->status != empty)
    {
        //Status ændres, varmen sættes til 0, og fugtigheden sættes til WET_TREE_HUMIDITY macroen.
        tree_to_water->status = wet;
        tree_to_water->humidity = WET_TREE_HUMIDITY;
        tree_to_water->heat = 0;
    }
}

//Når start er true starter brænden på disse x y koordinater
void start_fire(forest_t forest, int x, int y) {
    change_tree_at_coords(forest, burning, x, y);
}

double calculate_fire_prob(forest_t forest, int x, int y) {
    //Sandsynligheden for at et træ ikke brænder er som udgangspunkt 100%, så 1.
    double not_fire_prob = 1;
    // For hvert træ inden for SPREAD_RANGE tjekkes der om det påvirker det aktuelle træ.
    for (int i = -(int)SPREAD_RANGE; i <= SPREAD_RANGE; i++) {
        for (int j = -(int)SPREAD_RANGE; j <= SPREAD_RANGE; j++) {
            //Hvis både x- og y-værdien er indenfor arrayet.
            if (check_bounds(forest, x + j, y + i)) {
                //Så finder vi træet og udregner afstanden af vektoren fra det, til vores aktuelle træ.
                tree_t* tree = get_tree(forest, x + j, y + i);
                /* For at få den aktuelle vektor husker vi at når y stiger, går vi nedad i skoven,
                 * så dens værdi er allerede negativ. Ved også at gøre x negativ har vi den omvendte vektor,
                 * af den fra vores aktuelle træ til træet sandsynligheden kommer fra,
                 * ergo fra træet til vores aktuelle træ.
                 */
                vector_t distance = new_vector(-j, i);
                //Tjek om træet brænder, og distancen faktisk er mindre end eller lig med SPREAD_RANGE
                if (tree->status == burning && distance.length <= SPREAD_RANGE) {
                    /*Vi finder sandsynligheder for at træet ikke brænder, og ganger dem på for heat og wind,
                     * samt distance.
                     */
                    not_fire_prob *= 1 - heat_prob(tree->heat, distance.length) * HEAT_FACTOR;
                    not_fire_prob *= 1 - wind_prob(forest.wind, distance) * WIND_FACTOR;
                }
            }
        }
    }
    /*Vi tager til sidst og medregner det aktuelle træs humidity, før vi returnerer 1 - not_fire_prob,
     *for at få fire_prob*/
    return (1 - not_fire_prob) * (1 - humidity_not_prob(*get_tree(forest, x, y)));
}

void user_drop_water(forest_t forest, int size_of_splash, int x, int y) {
    //For alle træer inden for size_of_splash rundt om koordinatet.
    for (int i = -size_of_splash; i <= size_of_splash; i++) {
        for (int j = -size_of_splash; j <= size_of_splash; j++) {
            change_tree_at_coords(forest, wet, x + j, y + i);
        }
    }
}

void user_dead_zone(forest_t forest, int size_of_dead_zone, int x, int y) {
    for (int j = -size_of_dead_zone; j <= size_of_dead_zone; j++) {
        change_tree_at_coords(forest, empty, j + x, size_of_dead_zone + y);
        change_tree_at_coords(forest, empty, j + x, -size_of_dead_zone + y);

        change_tree_at_coords(forest, empty, size_of_dead_zone + x, j + y);
        change_tree_at_coords(forest, empty, -size_of_dead_zone + x, j + y);
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

void burndown(forest_t forest) {
    //For hvert træ i skoven
    for (int i = 0; i < forest.height; i++ ) {
        for (int j = 0; j < forest.width; j++) {
            tree_t* tree = get_tree(forest, j, i);
            if (tree->status == burning) {
                //Træet mister brændstof med RATE_OF_BURN.
                tree->fuel_left -= RATE_OF_BURN;
                heat_by_fuel_left(tree);
                //Hvis brændstof er 0 eller under, ændres status til burnt.
                if (tree->fuel_left <= 0) {
                    change_tree(tree, burnt);
            }
            }
        }
    }
}


int sim_finished_check(forest_t forest) {
    //Vi tjekker om nogen træer stadig brænder.
    int counter = get_trees_amount(forest, burning);
    if (counter > 0) {
        return 0;
    }
    return 1;
}

void spread(forest_t forest) {
    //Vi scanner for hvilke træer der skal brænde.
    int* trees_to_burn = scan_forest_spread(forest);
    if (trees_to_burn != NULL)
    {
        for (int i = 0; i < forest.size; i++) {
            //Hvis trees_to_burn returnerer sandt.
            if (trees_to_burn[i])
                change_tree(&forest.trees[i], burning);
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
                        trees_to_burn[counter] = 1;
                    }
                    else {
                        trees_to_burn[counter] = 0;
                    }
                    break;
                default:
                trees_to_burn[counter] = 0;

                break;
            }
            counter++;
        }
    }
    return trees_to_burn;
}
void tick(forest_t forest)
{
    //Vi vil have at brændende træer mister brændstof, og at ilden spreder sig.
    burndown(forest);
    //Hvis dette array ikke er en NULL pointer, fortsætter vi.
    spread(forest);
}

void fire_sim(forest_t forest, int* tickCounter) {
    //Vi gør den accept_user_input threaden klar, og sørger for at den er låst indtil programmet er afsluttet.
    pthread_mutex_t accept_user_input;
    pthread_mutex_init(&accept_user_input, NULL);
    pthread_mutex_lock(&accept_user_input);

    // Vi får output_bufferen
    CONSOLE_SCREEN_BUFFER_INFO output_buffer;
    GetConsoleScreenBufferInfo(hConsole, &output_buffer);
    COORD start_coord = output_buffer.dwCursorPosition;
    input_t user_input = {0, 0, none, start_coord.Y, 1, forest, &accept_user_input};

    pthread_t input_thread;
    pthread_create(&input_thread, NULL, user_input_loop, &user_input);

    DWORD current_fdwMode;
    GetConsoleMode(hConsole, &current_fdwMode);
    current_fdwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, current_fdwMode);

    do {
        //Hvis simulationen ikke er på pause.
        if (!user_input.paused)
        {
            //Tick fremad, tickcounter går op med 1.
            tick(forest);
            (*tickCounter)++;
        }
        //Vi printer skoven, efterfulgt at statusteksten
        print_forest(forest, start_coord.Y);
        status_text(forest,*tickCounter);

        //Vi beder computeren om at vente 0.4 sekunder (10^5 mikrosekunder * 4) mellem hver iteration
        usleep((useconds_t)pow(10,5) * 4);

        //Vi checker om simulationen er færdig
    } while (!sim_finished_check(forest));
    printf("Sim is finished!\n");
    //Vi låser op for accept_user_input, for at joine dem, så vi kan afslutte ordentligt.
    pthread_mutex_unlock(&accept_user_input);
    pthread_join(input_thread, NULL);
}

void status_text(forest_t forest, int tickCount) {
    //Sørger for at status tekst, er af normale farver
    reset_color();
    printf("Status:\n");
    print_wind(forest.wind);

    //Finder variable vi gerne vil vise.
    int fresh_count = get_trees_amount(forest, fresh);
    int burnt_count = get_trees_amount(forest, burnt);

    //De printes samt tickCount, som kommer udefra funktionen.
    printf("%11s: %4d\n", "Fresh trees", fresh_count);
    printf("%11s: %4d\n", "Burnt trees", burnt_count);
    printf("%11s: %4d\n", "Tick count", tickCount);
}

