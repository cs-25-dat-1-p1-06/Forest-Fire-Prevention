#include "../external/cJSON.h"
#include "fire-sim.h"
#include "input.h"
#include "console.h"
#include "wind.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    srand(time(NULL));

    FILE *fp = fopen("./logs/output.json","r");
    cJSON *root = NULL;

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
    //Stopper programmet hvis der ikke er nogle logs.
    if (!root) {
        printf("No logs found!");
        exit(EXIT_FAILURE);
    }

    //Select log
    cJSON *logsItem = cJSON_GetObjectItem(root, "logs");
    int logCount = logsItem->valueint;
    int selectedLog;

    //Input validation
    do {
        printf("Found %d logs. Select a log between 1-%d:\n", logCount,logCount);
        //Tjekker om input er en int
        if (scanf("%d", &selectedLog) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            // Clear input buffer for at undgå inf loop
            while (getchar() != '\n');
        }
    } while (selectedLog < 1 || selectedLog > logCount); // Scan indtil inputtet er i logcount range.

    char logName[32];
    sprintf(logName, "log%d",selectedLog);
    cJSON *logItem = cJSON_GetObjectItem(root, logName); //indlæst log

    cJSON * settingsItem = cJSON_GetArrayItem(logItem, 0); //index 0 for settings objekt
    cJSON* width = cJSON_GetObjectItem(settingsItem, "width");
    cJSON* height = cJSON_GetObjectItem(settingsItem, "height");
    cJSON* density = cJSON_GetObjectItem(settingsItem, "density");

    cJSON* windItem = cJSON_GetArrayItem(logItem,1); //index 1 for wind objekt
    cJSON* speed = cJSON_GetObjectItem(windItem,"speed");
    cJSON* x = cJSON_GetObjectItem(windItem,"x");
    cJSON* y = cJSON_GetObjectItem(windItem,"y");

    printf("To access different modes use the following keys:\n");
    printf("Forest Thinning: 'F'\n");
    printf("Water Drop: 'W'\n");
    printf("Dead Zone (size of 3): 'D'\n");
    printf("Pause: 'Space'\n");
    printf("%s: height: %d, width: %d, density: %.2lf\n", logName,height->valueint,width->valueint,density->valuedouble);

    console_setup();

    //Initiliasere vind fra json
    vector_t wind = new_vector(x->valuedouble,y->valuedouble);
    wind.length = speed->valuedouble;

    forest_t forest = make_rnd_forest(density->valuedouble, width->valueint, height->valueint, wind);

    cJSON_Delete(root);

    int fire_start_x, fire_start_y;
    do
    {
        fire_start_x = rand() % forest.width;
        fire_start_y = rand() % forest.height;

    } while (!(get_tree(forest, fire_start_x, fire_start_y)->status != empty));

    start_fire(forest, fire_start_x, fire_start_y);


    int tickCounter = 0;
    fire_sim(forest, &tickCounter);

    free(forest.trees);

    system("pause");

    return 0;
}
