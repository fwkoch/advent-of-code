#include <stdio.h>
#include "main.h"

int main()
{
    int mass;
    printf("Enter mass: ");
    scanf("%d", &mass);
    printf("Fuel: %d \n", mass_to_fuel(mass));
    return 0;
}

int mass_to_fuel(int mass)
{
    return mass/3 - 2;
}
