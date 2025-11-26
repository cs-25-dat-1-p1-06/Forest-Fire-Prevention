#ifndef FOREST_FIRE_PREVENTION_FIRESIM_H
#define FOREST_FIRE_PREVENTION_FIRESIM_H
#include <windows.h>
#include <wind.h>


typedef enum {empty, fresh, burning, burnt, wet} status_e;
typedef struct {
    status_e status;
    double humidity, fire_strength, fuel_left;
} tree_t;

/**
 * Funktion der laver en tilfældig skov, af 'fresh' eller 'empty' træer.
 * Og ændrer deres indhold til at passe dertil
 * @param forest Skoven
 * @param density Sandsynligheden for om et givet træ er 'fresh' eller 'empty'
 * @param size Størrelsen af skoven
 */
void make_rnd_forest(tree_t* forest, double density, int size);

/**
 * Funktion der printer skoven som farver i konsollen baseret på hvert træs status
 * @param forest Træernes array: Skoven.
 * @param height Hvor mange træer høj skoven er
 * @param width Hvor mange træer bred skoven er
 */
void print_forest(tree_t* forest, int height, int width);

/**
 * Funktion der får en tree pointer ud fra koordinater, for at hente info eller ændre i den.
 * @param x 1. koordinat
 * @param y 2. koordinat.
 * @param width Bredden bruges til at tjekke hvornår linjen skiftes
 * @param forest Skoven træet hentes fra
 * @return Pointer til træet
 */
tree_t* get_tree(int x, int y, int width, tree_t* forest);

/**
 * Funktion der tæller antal træer med en given status
 * @param forest Træernes array: Skoven
 * @param size Skovens størrelse. Height * width
 * @param target Status man leder efter
 * @return Antal træer med status.
 */
int get_trees_amount(tree_t* forest, int size, status_e target);

/**
 * Funktion der skriver hvor mange træer er friske og brændte, samt vindens styrke og retning.
 * @param wind Vindens retning og styrke
 * @param forest Træernes array: Skoven
 * @param size Størrelsen på skoven
 */
void status_text(wind_t* wind, tree_t* forest, int size);

/**
 * Funktion der checker om der stadig er ild i skoven.
 * @param forest Skoven
 * @param size Skovens størrelse
 * @return Hvis noget brænder 1 ellers 0
 */
int sim_finished_check(tree_t* forest, int size);

/**
 * Ændrer et givet træ til at brænde med en fast styrke. Sådan starter vi simulationen
 * @param forest Skoven
 * @param x 1. koordinat til træet som skal brænde
 * @param y 2. koordinat til træet som skal brænde
 * @param width Skovens bredde i træer
 */
void start_fire(tree_t* forest, int x, int y, int width);

/**
 * gennemgår de træer omkring et givet træ og ser hvor meget fire strength der er i dem og returnerer den samlede fire strength
 * @param forest hele skoven
 * @param x x koordinat til det træ hvis omgivelser der skal tjekkes
 * @param y x koordinat til det træ hvis omgivelser der skal tjekkes
 * @param width bredden på skoven
 * @param height højden på skoven
 * @return
 */
int check_surrounding_fire_strength(tree_t* forest, int x, int y,int width, int height);

/**
 * Givet et træ, ændres den og de rundtomstående træer til at være våde.
 * @param forest Skoven
 * @param x 1. koordinat
 * @param y 2. koordinat
 * @param width Skovens bredde
 */
void user_drop_water(tree_t* forest, int x, int y, int width);

/**
 * Funktion der laver en firkant rundt om et givet træ af træer med status "empty",
 * givet en størrelse af firkanten
 * @param forest Skoven
 * @param x 1. koordinat
 * @param y 2. koordinat
 * @param width Skovens bredde i træer
 * @param size_of_dead_zone Antal træer fra midten i hver retning ("Radius" på firkanten).
 */
void user_dead_zone(tree_t* forest, int x, int y, int width, int size_of_dead_zone);

/**
 * Funktion der kører simulationen. Den gør alt der sker inden for et tick i computeren.
 * Og gentager dette skridt indtil simulationen er færdig. Parametre anvendes i de indre funktioner.
 * @param forest Skoven
 * @param height Skovens højde
 * @param width Skovens bredde
 * @param wind Vindens styrke og retning
 * @param start_y Hvor konsollens cursor er, når denne funktion starter
 */
void tick(tree_t* forest, int height, int width, wind_t* wind, int start_y);

/**
 * Checker hvilke træer der brænder, og sænker fuel_left med en fast værdi.
 * Hvis fuel_left kommer til eller under 0, ændres træets status til burnt, og fire_strength til 0.
 * @param forest Skoven
 * @param height Skovens højde
 * @param width Skovens bredde
 */
void burndown(tree_t* forest, int height, int width);

/**
 * Funktion der givet skovens tilstand og et træ, tjekker hvor vidt træet vil brænde.
 * @param forest Skoven
 * @param x 1. koordinat på træ der skal tjekkes om det brænder
 * @param y 2. koordinat på træ
 * @param width Skovens bredde
 * @param height Skovens højde
 * @return Chance: En double der siger træets risiko for at brænde i procenter fra 1 til 100.
 */
double calculate_risk_of_burning(tree_t* forest, int x, int y,int width, int height);

/**
 * Funktion der for hvert træ i skoven tjekker om det skal brændes vha. trees_to_burn.
 * Hvis trees_to_burn er en NULL pointer eller giver værdien -1, fortsættes,
 * ellers sættes træet til burning med en fast fire_strength.
 * @param forest Skovens tilstand
 * @param height Skovens højde
 * @param width Skovens bredde
 * @param trees_to_burn Et array af alle træer som skal sættes ændres til at brænde
 */
void spread(tree_t* forest, int height, int width, int* trees_to_burn);

/**
 * Funktion der givet en skov checker vha. andre funktioner hvilke træer,
 * som burde sættes til at brænde.
 * @param forest Skoven
 * @param height Skovens højde
 * @param width Skovens bredde
 * @return Array af integers som er værdier til et træs plads i arrayet (skoven)
 */
int* scan_forest_spread(tree_t* forest, int height, int width);

#endif //FOREST_FIRE_PREVENTION_FIRESIM_H