#include <stdio.h>
#include <stdlib.h>
#include "main.h"

int main(int argc, char* argv[])
{
    char const* const file_name = argv[1];
    FILE* file = fopen(file_name, "r");
    char line[256];
    int total_fuel = 0;
    while (fgets(line, sizeof(line), file)) {
        int mass = atoi(line);
        int fuel = mass_to_fuel(mass);
        total_fuel = total_fuel + fuel;
    }
    fclose(file);
    printf("%d\n", total_fuel);
    return 0;
}

int mass_to_fuel(int mass)
{
    int fuel = mass/3 - 2;
    if (fuel < 0) {
        return 0;
    }
    return fuel + mass_to_fuel(fuel);
}
