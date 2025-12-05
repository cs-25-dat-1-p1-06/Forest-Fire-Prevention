#include <../external/cJSON.h>
#include <stdio.h>
#include <fire-sim.h>

void write_output(forest_t forest, int tickCount,int width,int height, double density) {

    // Laver et cJSON objekt til output
    cJSON *output = cJSON_CreateObject();
    cJSON *settings; //objekt til settings værdier
    cJSON *results; //objekt til results værdier

    cJSON_AddItemToObject(output, "settings", settings = cJSON_CreateObject());
    cJSON_AddNumberToObject(settings,"width",width);
    cJSON_AddNumberToObject(settings,"height",height);
    cJSON_AddNumberToObject(settings,"density",density);
    cJSON_AddItemToObject(output, "results", results = cJSON_CreateObject());
    cJSON_AddNumberToObject(results, "fresh", get_trees_amount(forest,fresh));
    cJSON_AddNumberToObject(results, "burnt", get_trees_amount(forest,burnt));
    cJSON_AddNumberToObject(output, "ticks", tickCount);

    // Konveterer cJSON objektet til en JSON streng
    char *json_str = cJSON_Print(output);

    // Udksriv JSON strengen til en fil
    FILE *fp = fopen("./logs/output.json", "w");
    if (fp == NULL) {
        printf("Error: Unable to open the file.\n");
        return;
    }
    fputs(json_str, fp);
    // Frigiv JSON strengen og cJSON objekt fra hukommelsen
    cJSON_free(json_str);
    cJSON_Delete(output);
}