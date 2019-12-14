#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char* argv[])
{
    char* file_name = argv[1];
    Chemical* chemicals = malloc(96 * sizeof(Chemical));

    char line[96];
    int num_chemicals, success;
    FILE* file;
    long request = 0;
    long increment = 1000000000000;
    while (1) {
        num_chemicals = 0;
        file = fopen(file_name, "r");
        while (fgets(line, sizeof(line), file)) {
            chemical_from_line(line, &num_chemicals, chemicals);
        }
        fclose(file);
        Chemical* fuel = find_chemical_by_name("FUEL ", &num_chemicals, chemicals);
        Chemical* ore = find_chemical_by_name("ORE  ", &num_chemicals, chemicals);
        ore->cache = 1000000000000;
        printf("Asking for: %ld FUEL\n", request + increment);
        success = fill_chemical_cache(fuel, request + increment);
        if (success) {
            printf("Success! Remaining ore: %ld\n", ore->cache);
            request += increment;
            continue;
        }
        printf("Failure.\n");
        if (increment > 1) {
            increment /= 10;
            continue;
        }
        break;
    }
    return 0;
}

void chemical_from_line(char *line, int *num_chemicals, Chemical *chemicals)
{
    char* name = malloc(5 * sizeof(char));
    int* input_quantities = malloc(8 * sizeof(int));
    Chemical** input_chemicals = malloc(8 * sizeof(Chemical *));
    Chemical* output_chemical;
    int amount, j;
    int i = 0;
    int input_ct = 0;
    while (line[i] != '=') {
        amount = 0;
        while (line[i] != ' ') {
            amount = amount*10 + (line[i] - '0');
            i++;
        }
        input_quantities[input_ct] = amount;
        i++;
        j = 0;
        strncpy(name, "     ", 5);
        while (line[i] != ',' && line[i] != ' ') {
            strncpy(&name[j], &line[i], 1);
            i++;
            j++;
        }
        input_chemicals[input_ct] = find_chemical_by_name(name, num_chemicals, chemicals);
        if (line[i] == ',') {
            i++;
        }
        input_ct++;
        i++;
    }
    i += 3;
    amount = 0;
    while (line[i] != ' ') {
        amount = amount*10 + (line[i] - '0');
        i++;
    }
    i++;
    j = 0;
    strncpy(name, "     ", 5);
    while (line[i] != '\n') {
        strncpy(&name[j], &line[i], 1);
        i++;
        j++;
    }
    output_chemical = find_chemical_by_name(name, num_chemicals, chemicals);
    output_chemical->output_quantity = amount;
    output_chemical->num_inputs = input_ct;
    output_chemical->input_quantities = input_quantities;
    output_chemical->input_chemicals = input_chemicals;
    output_chemical->cache = 0;
}

Chemical* find_chemical_by_name(char *name, int *num_chemicals, Chemical *chemicals)
{
    for (int i = 0; i < *num_chemicals; i++) {
        if (strncmp(chemicals[i].name, name, 5) == 0) {
            return &chemicals[i];
        }
    }
    strncpy(chemicals[*num_chemicals].name, name, 5);
    *num_chemicals = *num_chemicals + 1;
    return &chemicals[*num_chemicals - 1];
}

int fill_chemical_cache(Chemical *chemical, long amount)
{
    int success;
    if (strncmp(chemical->name, "ORE", 3) == 0) {
        if (chemical->cache < amount) {
            return 0;
        }
        return 1;
    }
    if (chemical->cache < amount) {
        long n_reactions = (amount - chemical->cache + chemical->output_quantity - 1) / chemical->output_quantity;
        for (int i = 0; i < chemical->num_inputs; i++) {
            success = fill_chemical_cache(chemical->input_chemicals[i], n_reactions * chemical->input_quantities[i]);
            if (!success) {
                return 0;
            }
            remove_from_cache(chemical->input_chemicals[i], n_reactions * chemical->input_quantities[i]);
        }
        chemical->cache += n_reactions * chemical->output_quantity;
    }
    return 1;
}

void remove_from_cache(Chemical *chemical, long amount)
{
    chemical->cache -= amount;
}

void print_chemical(Chemical* chemical)
{
    printf("%d %s <=", chemical->output_quantity, chemical->name);
    for (int i = 0; i < chemical->num_inputs; i++) {
        printf(" %d %s", chemical->input_quantities[i], chemical->input_chemicals[i]->name);
    }
    printf("\n");
}
