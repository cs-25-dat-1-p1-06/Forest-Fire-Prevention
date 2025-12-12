#include <../external/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <fire-sim.h>

void write_output(forest_t forest, int tickCount, int width, int height, double density) {

    //Læser indhold i log fil
    FILE *fp = fopen("./logs/output.json", "r");
    cJSON *root = NULL; //Root objekt der holder alle logs

    if (fp) {
        //Går til slutningen af filen og finder størrelsen så der kan allokeres nok data
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        //Går tilbage til starten så filen kan læses fra start
        fseek(fp, 0, SEEK_SET);

        char *data = malloc(size + 1);
        fread(data, 1, size, fp);
        data[size] = '\0';

        fclose(fp);
        root = cJSON_Parse(data); //Tilføjer eksisterende indhold som json objekt
        free(data);
    }

    //Sæt logcount til 0 hvis filen ikke findes.
    if (!root) {
        root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "logs", 0);
    }

    //Henter og opdatere logs count
    cJSON *logsItem = cJSON_GetObjectItem(root, "logs");
    int logCount = logsItem->valueint;
    logCount++;
    cJSON_ReplaceItemInObject(root, "logs", cJSON_CreateNumber(logCount));

    //Opret ny log
    char logName[32];
    sprintf(logName, "log%d", logCount);

    cJSON *logObj = cJSON_CreateObject();
    cJSON_AddItemToObject(root, logName, logObj); //Tilføj ny log til root

    //Settings
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddNumberToObject(settings, "width", width);
    cJSON_AddNumberToObject(settings, "height", height);
    cJSON_AddNumberToObject(settings, "density", density);
    cJSON_AddItemToObject(logObj, "settings", settings);

    //Results
    cJSON *results = cJSON_CreateObject();
    cJSON_AddNumberToObject(results, "fresh", get_trees_amount(forest, fresh));
    cJSON_AddNumberToObject(results, "burnt", get_trees_amount(forest, burnt));
    cJSON_AddItemToObject(logObj, "results", results);

    //Tick count
    cJSON_AddNumberToObject(logObj, "ticks", tickCount);

    //Skriv tilbage til fil
    char *json_str = cJSON_Print(root);
    fp = fopen("./logs/output.json", "w");
    if (!fp) {
        printf("Error: Unable to write file.\n");
        return;
    }
    fputs(json_str, fp);
    fclose(fp);

    //Oprydning
    cJSON_free(json_str);
    cJSON_Delete(root);
}