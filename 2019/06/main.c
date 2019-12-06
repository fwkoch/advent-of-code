#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"


int main(int argc, char* argv[])
{
    // hcreate(3000);
    char const* const file_name = argv[1];
    FILE* file = fopen(file_name, "r");
    char line[32];
    struct Planet planets[3000];

    int ct = 0;
    while (fgets(line, sizeof(line), file)) {
        // struct Planet* center = (struct Planet*)malloc(sizeof(struct Planet));

        char center_name[4] = {line[0], line[1], line[2], '\0'};
        char satellite_name[4] = {line[4], line[5], line[6], '\0'};

        // printf("processing %s ) %s\n", center_name, satellite_name);

        int center_exists = 0;
        int satellite_exists = 0;
        struct Planet* center;
        struct Planet* satellite;
        for (int i = 0; i < ct; i++) {
            if (strncmp(planets[i].name, center_name, 3) == 0) {
                center_exists = 1;
                center = &planets[i];
            }
            if (strncmp(planets[i].name, satellite_name, 3) == 0) {
                satellite_exists = 1;
                satellite = &planets[i];
            }
        }
        if (!center_exists) {
            // printf("Creating planet number %d - %s\n", ct, center_name);
            strncpy(planets[ct].name, center_name, 3);
            planets[ct].direct_orbits = 0;
            center = &planets[ct];
            ct++;
        }
        if (!satellite_exists) {
            // printf("Creating planet number %d - %s\n", ct, satellite_name);
            strncpy(planets[ct].name, satellite_name, 3);
            planets[ct].direct_orbits = 0;
            satellite = &planets[ct];
            ct++;
        }

        center->direct_orbiters[center->direct_orbits] = satellite;
        center->direct_orbits++;

        printf("center: %s  - orbiters:", center->name);
        for (int i = 0; i < center->direct_orbits; i++) {
            printf(" %s", center->direct_orbiters[i]->name);
        }
        printf("\n");
    }
    fclose(file);

    printf("%d\n", ct);
    int com_index = 0;
    char com[3] = "COM";
    while (com_index < ct) {
        if (strncmp(planets[com_index].name, com, 3) == 0) {
            break;
        } else {
            com_index++;
            continue;
        }
        printf("%s not found\n", com);
        return 1;
    }

    int orbits = count_total_orbits(planets[com_index]);
    printf("Total orbits: %d\n", orbits);

    return 0;
}

int count_total_orbiters(struct Planet planet)
{
    int total_orbiters = 0;
    for (int i = 0; i < planet.direct_orbits; i++) {
        total_orbiters += 1 + count_total_orbiters(*planet.direct_orbiters[i]);
    }
    return total_orbiters;
}

int count_total_orbits(struct Planet planet)
{
    int total_orbits = count_total_orbiters(planet);
    for (int i = 0; i < planet.direct_orbits; i++) {
        total_orbits += count_total_orbits(*planet.direct_orbiters[i]);
    }
    return total_orbits;
}
