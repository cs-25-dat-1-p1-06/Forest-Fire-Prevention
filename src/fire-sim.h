
#ifndef FOREST_FIRE_PREVENTION_FIRESIM_H
#define FOREST_FIRE_PREVENTION_FIRESIM_H
#include <wind.h>
  
#define RATE_OF_BURN 0.2
#define STARTING_HEAT 30
#define TREE_FUEL 1.0
#define SPREAD_HEAT 15
#define SPREAD_RANGE 2.3
#define HEAT_FACTOR 0.1
#define USER_DEAD_ZONE_SIZE 3
#define USER_SPLASH_ZONE_SIZE 2
#define WET_TREE_HUMIDITY 100


typedef enum {empty, fresh, burning, burnt, wet} status_e;
typedef struct {
    status_e status;
    double humidity, heat, fuel_left;
} tree_t;

typedef struct
{
    tree_t* trees;
    int width, height, size;
    vector_t wind;
} forest_t;

/**
 * Funktion der laver en tilfældig skov, af 'fresh' eller 'empty' træer.
 * Og ændrer deres indhold til at passe dertil
 * @param density Sandsynligheden for om et givet træ er 'fresh' eller 'empty'
 */
forest_t make_rnd_forest(double density, int width, int height, vector_t wind);

/**
 * Funktion der printer skoven som farver i konsollen baseret på hvert træs status
 * @param forest Træernes array: Skoven.
 * @param start_y hvor langt ned i konsollen skoven skal udskrives
 */
void print_forest(forest_t forest, short start_y);

void print_tree(tree_t tree);


void change_tree(forest_t forest, status_e new_status, int x, int y);
void destroy_tree(tree_t* tree_to_destroy);
void create_tree(tree_t* tree_to_create);
void burn_tree(tree_t* tree_to_burn);
void finish_burn(tree_t* tree_to_burnt);
void water_tree(tree_t* tree_to_water);


/**
 * Funktion der får en tree pointer ud fra koordinater, for at hente info eller ændre i den.
 * @param x 1. koordinat
 * @param y 2. koordinat.
 * @param forest Skoven træet hentes fra
 * @return Pointer til træet
 */
tree_t* get_tree(forest_t forest, int x, int y);

/**
 * Funktion der tæller antal træer med en given status
 * @param forest Træernes array: Skoven
 * @param target Status man leder efter
 * @return Antal træer med status.
 */
int get_trees_amount(forest_t forest, status_e target);

/**
 * Funktion der skriver hvor mange træer er friske og brændte, samt vindens styrke og retning.
 * @param forest Skoven
 */
void status_text(forest_t forest,int tickCount);

/**
 * Funktion der checker om der stadig er ild i skoven.
 * @param forest Skoven
 * @return Hvis noget brænder 1 ellers 0
 */
int sim_finished_check(forest_t forest);
/**
 * Ændrer et givet træ til at brænde med en fast styrke. Sådan starter vi simulationen
 * @param forest Skoven
 * @param x 1. koordinat til træet som skal brænde
 * @param y 2. koordinat til træet som skal brænde
 */
void start_fire(forest_t forest, int x, int y);

/**
 * gennemgår de træer omkring et givet træ og ser hvor meget fire strength der er i dem og returnerer den samlede fire strength
 * @param forest hele skoven
 * @param x x koordinat til det træ hvis omgivelser der skal tjekkes
 * @param y x koordinat til det træ hvis omgivelser der skal tjekkes
 * @return
 */
double calculate_fire_prob(forest_t forest, int x, int y);

/**
 * Givet et træ, ændres den og de rundtomstående træer til at være våde.
 * @param forest Skoven
 * @param x 1. koordinat
 * @param y 2. koordinat
 */
void user_drop_water(forest_t forest, int size_of_splash, int x, int y);

/**
 * Funktion der laver en firkant rundt om et givet træ af træer med status "empty",
 * givet en størrelse af firkanten
 * @param forest Skoven
 * @param x 1. koordinat
 * @param y 2. koordinat
 * @param width Skovens bredde i træer
 * @param size_of_dead_zone Antal træer fra midten i hver retning ("Radius" på firkanten).
 */
void user_dead_zone(forest_t forest, int size_of_dead_zone, int x, int y);

/**
 * Alt det som skal ske inden for et tick i simulationen
 * @param forest Skoven
 */
void tick(forest_t forest);
/**
 * Funktion der kører simulationen. Gentager tick funktionen indtil simulationen er færdig med et delay på 0,1 sekunder mellem hvert tick
 * @param forest Skoven
 * @param start_y Hvor konsollens cursor var, efter brugeren indtaster data
 */
void fire_sim(forest_t forest, int* tickCounter, short start_y);

/**
 * Checker hvilke træer der brænder, og sænker fuel_left med en fast værdi.
 * Hvis fuel_left kommer til eller under 0, ændres træets status til burnt, og fire_strength til 0.
 * @param forest Skoven
 */
void burndown(forest_t forest);


/**
 * Funktion der for hvert træ i skoven tjekker om det skal brændes vha. trees_to_burn.
 * Hvis trees_to_burn er en NULL pointer eller giver værdien -1, fortsættes,
 * ellers sættes træet til burning med en fast fire_strength.
 * @param forest Skovens tilstand
 * @param trees_to_burn Et array af alle træer som skal sættes ændres til at brænde
 */
void spread(forest_t forest, int* trees_to_burn);

/**
 * Funktion der givet en skov checker vha. andre funktioner hvilke træer,
 * som burde sættes til at brænde.
 * @param forest Skoven
 * @return Array af integers som er værdier til et træs plads i arrayet (skoven)
 */


int* scan_forest_spread(forest_t forest);
int check_bounds(forest_t forest, int x, int y);

#endif //FOREST_FIRE_PREVENTION_FIRESIM_H