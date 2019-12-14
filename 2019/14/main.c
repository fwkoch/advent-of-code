#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char* argv[])
{
    Chemical* chemicals = malloc(96 * sizeof(Chemical));
    int num_chemicals = 0;

    char* file_name = argv[1];
    FILE* file = fopen(file_name, "r");
    char line[96];
    while (fgets(line, sizeof(line), file)) {
        chemical_from_line(line, &num_chemicals, chemicals);
    }
    fclose(file);
    for (int i = 0; i < num_chemicals; i++) {
        print_chemical(&chemicals[i]);
    }
    Chemical* fuel = find_chemical_by_name("FUEL ", &num_chemicals, chemicals);
    ask_for_chemical(fuel, 1);
    Chemical* ore = find_chemical_by_name("ORE  ", &num_chemicals, chemicals);
    printf("Total ore: %d\n", ore->cache);
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

void ask_for_chemical(Chemical *chemical, int amount)
{

    if (strncmp(chemical->name, "ORE", 3) == 0) {
        chemical->cache += amount;
        return;
    }
    while (chemical->cache < amount) {
        for (int i = 0; i < chemical->num_inputs; i++) {
            ask_for_chemical(chemical->input_chemicals[i], chemical->input_quantities[i]);
        }
        chemical->cache += chemical->output_quantity;
    }
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
