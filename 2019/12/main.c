#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char* argv[])
{
    Moon moons_init[4];
    Moon moons[4];
    char* file_name = argv[1];
    FILE* file = fopen(file_name, "r");
    char line[32];
    for (int i = 0; i < 4; i++) {
        fgets(line, sizeof(line), file);
        initialize_moon(line, &moons_init[i]);
        initialize_moon(line, &moons[i]);
    }
    fclose(file);
    // int num_steps = 1000;
    // print_moons(moons);
    // for (int i = 0; i < num_steps; i++) {
    //     update_velocities(moons);
    //     update_positions(moons);
    // }
    // print_moons(moons);
    // int energy = calculate_energy(moons);
    // printf("Total energy: %d\n", energy);
    long intervals[3];
    long ct;
    for (int dim = 0; dim < 3; dim++) {
        ct = 0;
        do
        {
            update_dimension_velocities(moons, dim);
            update_dimension_positions(moons, dim);
            ct++;
        } while (compare_moons(moons, moons_init, dim) == 0);
        intervals[dim] = ct;
    }
    printf("Intervals: [%ld, %ld, %ld] ", intervals[0], intervals[1], intervals[2]);
    printf("LCM: %ld\n", least_common_multiple(intervals[0], intervals[1], intervals[2]));
    return 0;
}

void print_moons(Moon* moons)
{
    for (int i = 0; i < 4; i++) {
        printf("pos=<x=%4d, y=%4d, z=%4d>, ", moons[i].position[0], moons[i].position[1], moons[i].position[2]);
        printf("vel=<x=%4d, y=%4d, z=%4d>\n", moons[i].velocity[0], moons[i].velocity[1], moons[i].velocity[2]);
    }
}

long least_common_multiple(long a, long b, long c)
{
    long lcm = a * b * c;
    long temp_lcm;
    long max;
    if (a >= b && a >= c) {
        max = a;
    } else if (b >= c) {
        max = b;
    } else {
        max = c;
    }
    for (long factor = max; factor > 1; factor--) {
        if (a % factor != 0 && b % factor != 0 && c % factor != 0) {
            continue;
        }
        temp_lcm = lcm / factor;
        if (temp_lcm % a == 0 && temp_lcm % b == 0 && temp_lcm % c == 0) {
            lcm = temp_lcm;
        }
    }
    return lcm;
}

int compare_moons(Moon* moons, Moon* moons_init, int dim)
{
    for (int i = 0; i < 4; i++) {
        if (moons[i].position[dim] != moons_init[i].position[dim]) {
            return 0;
        }
        if (moons[i].velocity[dim] != moons_init[i].velocity[dim]) {
            return 0;
        }
    }
    return 1;
}

int potential_energy(Moon* moon) {
    int energy = 0;
    for (int dim = 0; dim < 3; dim++) {
        energy += abs(moon->position[dim]);
    }
    return energy;
}

int kinetic_energy(Moon* moon) {
    int energy = 0;
    for (int dim = 0; dim < 3; dim++) {
        energy += abs(moon->velocity[dim]);
    }
    return energy;
}

int calculate_energy(Moon* moons) {
    int energy = 0;
    for (int i = 0; i < 4; i++) {
        energy += potential_energy(&moons[i]) * kinetic_energy(&moons[i]);
    }
    return energy;
}

void update_velocities(Moon* moons)
{
    for (int dim = 0; dim < 3; dim++) {
        update_dimension_velocities(moons, dim);
    }
}

void update_dimension_velocities(Moon* moons, int dim)
{
    for (int i = 0; i < 4; i++) {
        for (int j = i; j < 4; j++) {
            if (moons[i].position[dim] > moons[j].position[dim]) {
                moons[i].velocity[dim]--;
                moons[j].velocity[dim]++;
            } else if (moons[i].position[dim] < moons[j].position[dim]){
                moons[i].velocity[dim]++;
                moons[j].velocity[dim]--;
            }
        }
    }
}

void update_positions(Moon* moons)
{
    for (int dim = 0; dim < 3; dim++) {
        update_dimension_positions(moons, dim);
    }
}

void update_dimension_positions(Moon* moons, int dim)
{
    for (int i = 0; i < 4; i++) {
        moons[i].position[dim] += moons[i].velocity[dim];
    }
}

void initialize_moon(char* line, Moon* moon)
{
    int i = 0;
    int mult;
    int value;
    for (int dim = 0; dim < 3; dim++) {
        mult = 1;
        value = 0;
        while (line[i] != '=') {
            i++;
        }
        i++;
        if (line[i] == '-') {
            mult = -1;
            i++;
        }
        while (line[i] != ',' && line[i] != '>') {
            value = 10 * value + (line[i] - '0');
            i++;
        }
        moon->position[dim] = mult * value;
        moon->velocity[dim] = 0;
    }
}
