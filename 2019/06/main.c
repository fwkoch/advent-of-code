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

        // printf("center: %s  - orbiters:", center->name);
        // for (int i = 0; i < center->direct_orbits; i++) {
        //     printf(" %s", center->direct_orbiters[i]->name);
        // }
        // printf("\n");
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

    // int orbits = count_total_orbits(planets[com_index]);
    // printf("Total orbits: %d\n", orbits);

    // int steps = count_steps_outward(planets[com_index], "YOU");
    // printf("Steps to YOU: %d\n", steps);

    int distance = find_shortest_distance(planets, ct, "YOU", "SAN");
    printf("Shortest path: %d\n", distance);
    printf("Total transfers: %d\n", distance-2);

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

int count_steps_outward(struct Planet start, char end[3])
{
    if (strncmp(start.name, end, 3) == 0) {
        return 0;
    }
    int steps = -1;
    for (int i = 0; i < start.direct_orbits; i++) {
        struct Planet orbiter = *start.direct_orbiters[i];
        int count = count_steps_outward(orbiter, end);
        if (count < 0) {
            continue;
        }
        steps = 1 + count;
        break;
    }
    return steps;
}

int find_shortest_distance(struct Planet* planets, int num_planets, char node_one[3], char node_two[3])
{

    if (strncmp(node_one, node_two, 3) == 0) {
        return 0;
    }
    struct Planet* common_ancestor = NULL;
    int distance = 0;
    for (int i = 0; i < num_planets; i++) {
        printf("Checking planet %s\n", planets[i].name);
        int node_one_distance = count_steps_outward(planets[i], node_one);
        printf("Distance to %s: %d\n", node_one, node_one_distance);
        if (node_one_distance < 0) {
            continue;
        }
        int node_two_distance = count_steps_outward(planets[i], node_two);
        printf("Distance to %s: %d\n", node_two, node_two_distance);
        if (node_two_distance < 0) {
            continue;
        }
        int total_distance = node_one_distance + node_two_distance;
        printf("Found common ancestor: %s - distance: %d", planets[i].name, total_distance);
        if (!common_ancestor || total_distance < distance) {
            printf(" - New closest\n");
            common_ancestor = &planets[i];
            distance = total_distance;
        } else {
            printf("\n");
        }
    }
    return distance;
}
