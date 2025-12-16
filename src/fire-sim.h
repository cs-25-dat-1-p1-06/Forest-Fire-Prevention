
#ifndef FOREST_FIRE_PREVENTION_FIRESIM_H
#define FOREST_FIRE_PREVENTION_FIRESIM_H
#include <wind.h>
  
#define RATE_OF_BURN 0.1
#define MAX_HEAT 0.3
#define SPREAD_RANGE 5
#define USER_DEAD_ZONE_SIZE 5
#define USER_SPLASH_ZONE_SIZE 2
#define WET_TREE_HUMIDITY 100
#define TREE_FUEL 1.0
#define STARTING_HUMIDITY 0
#define HEAT_FACTOR 1
#define WIND_FACTOR 1


//Forskellige statuser et træ kan have defineres som enums.
typedef enum {empty, fresh, burning, burnt, wet} status_e;

//Træ oprettes som struct, med en status, fugtighed, varme (mens det brænder) og brændstof tilbage.
typedef struct {
    status_e status;
    double humidity, heat, fuel_left;
} tree_t;

//Skoven oprettes som et struct, med tree_t array, bredde, højde og størrelse, og vinden som en vektor.
typedef struct
{
    tree_t* trees;
    int width, height, size;
    vector_t wind;
} forest_t;

/**
 * Funktion der laver en tilfældig skov, af 'fresh' eller 'empty' træer ud fra den givne højde, bredde, og tæthed.
 * Tildeler også vinden
 * @param density Skovens tæthed
 * @param width Skovens bredde
 * @param height Skovens højde
 * @param wind Vinden vektor
 * @return Den tilfældige skov
 */
forest_t make_rnd_forest(double density, int width, int height, vector_t wind);

/**
 * Funktion der printer skoven med farver i konsollen baseret på hvert træs status
 * @param forest Skoven
 * @param start_y Hvor langt ned i konsollen skoven skal udskrives
 */
void print_forest(forest_t forest, short start_y);

/**
 * Funktion der givet et træ, printer et træ med en baggrundsfarve farve, baseret på træets status.
 * @param tree træet der skal printes.
 */
void print_tree(tree_t tree);

/**
 * Funktion der ændrer et træs status ved givne koordinater
 * @param forest Skoven
 * @param new_status Den nye status træet skal have
 * @param x Træets x koordinat
 * @param y Træets y koordinat
 */
void change_tree_at_coords(forest_t forest, status_e new_status, int x, int y);

/**
 * Funktion der ændrer et træs status
 * @param tree_to_change Pointer til træ der skal ændres
 * @param new_status Status træet ændres til
 */
void change_tree(tree_t* tree_to_change, status_e new_status);

/**
 * Funktion der ændre status til empty og tilpasser værdierne
 * @param tree_to_destroy Pointer til træ der skal slettes
 */
void destroy_tree(tree_t* tree_to_destroy);

/**
 * Funktion der ændre status til fresh og tilpasser værdierne
 * @param tree_to_create Pointer til træ der skal oprettes.
 */
void create_tree(tree_t* tree_to_create);

/**
 * Funktion der ændre status til burning og tilpasser værdierne
 * @param tree_to_burn Pointer til træ der skal brændes.
 */
void burn_tree(tree_t* tree_to_burn);

/**
 * Funktion der ændre status til burnt og tilpasser værdierne
 * @param tree_to_burnt Pointer til træ der skal brænde ud.
 */
void finish_burn(tree_t* tree_to_burnt);

/**
 * Funktion der ændre status til wet og tilpasser værdierne
 * @param tree_to_water Pointer til træ der skal vandes.
 */
void water_tree(tree_t* tree_to_water);

/**
 * Hjælpefunktion der får en tree pointer ud fra koordinater, for at hente info eller ændre i den.
 * @param x x koordinat til træet
 * @param y y koordinat til træet
 * @param forest Skoven træet hentes fra
 * @return Pointer til træet
 */
tree_t* get_tree(forest_t forest, int x, int y);

/**
 * Hjælpefunktion der tæller antal træer med en given status.
 * @param forest Skoven
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
 * Ændrer et givet træ til at brænde. Sådan starter vi simulationen
 * @param forest Skoven
 * @param x x koordinat til træet som skal brænde
 * @param y y koordinat til træet som skal brænde
 */
void start_fire(forest_t forest, int x, int y);

/**
 * Funktion der givet et træ i skoven, returnerer en sandsynlighed for at træet skal brænde,
 * baseret på vinden, de omkringstående træer, og det aktuelle træs fugtighed.
 * @param forest Skoven
 * @param x x koordinat til det aktuelle træ.
 * @param y y koordinat til det aktuelle træ.
 * @return Sandsynlighed for at træet brænder, mellem 0 og 1
 */
double calculate_fire_prob(forest_t forest, int x, int y);

/**
 * Funktion der givet et træ, ændre den og de omkringliggende træer til at være våde.
 * @param forest Skoven
 * @param x x koordinat til centrum for vand splash
 * @param y y koordinat til centrum for vand splash
 */
void user_drop_water(forest_t forest, int size_of_splash, int x, int y);

/**
 * Funktion der tegner en firkant med sidelængder size_of_dead_zone * 2 + 1 rundt om et givet træ hvor siderne er
 * af træer med status "empty"
 * @param forest Skoven
 * @param size_of_dead_zone sidelængden på firkanten divideret med 2 og minus 1
 * @param x x koordinat til træet midt i zonen.
 * @param y y koordinat til træet midt i zonen.
 */
void user_dead_zone(forest_t forest, int size_of_dead_zone, int x, int y);

/**
 * Funktion der gør alt som skal ske inden for et tick af simulationen.
 * @param forest Skoven
 */
void tick(forest_t forest);

/**
 * Funktion der kører simulationen. Gentager tick funktionen indtil simulationen er færdig.
 * @param forest Skoven
 * @param tickCounter Pointer til counter, som stiger hver gang et tick udføres.
 */
void fire_sim(forest_t forest, int* tickCounter);

/**
 * Funktion der tjekker hvilke træer der brænder, og sænker fuel_left med en fast værdi.
 * Hvis fuel_left kommer til at være mindre end eller lig med 0, ændres træets status til burnt, og fire_strength til 0.
 * @param forest Skoven
 */
void burndown(forest_t forest);

/**
 * ændrer mængden af varme i et brændende træ baseret på mængden af brændstof der er tilbage
 * @param tree det brændende træ
 */
void heat_by_fuel_left(tree_t* tree);

/**
 * Funktion spreder ilden til de træer der burde brænde
 * @param forest Skovens tilstand
 */
void spread(forest_t forest);

/**
 * Funktion der givet en skov tjekker vha. andre funktioner hvilke træer,
 * som burde sættes til at brænde.
 * @param forest Skoven
 * @return Array af integers for hvert træ i skoven. 1 hvis det skal brænde ellers 0.
 */
int* scan_forest_spread(forest_t forest);

/**
 * Hjælpefunktion der tjekker om givne koordinater til et træ ligger inden for skovens grænser
 * @param forest skoven
 * @param x x koordinat
 * @param y y koordinat
 * @return 1 hvis træet er inden for grænserne. 0 hvis ikke
 */
int check_bounds(forest_t forest, int x, int y);

#endif //FOREST_FIRE_PREVENTION_FIRESIM_H