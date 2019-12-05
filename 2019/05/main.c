#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char* argv[])
{
    int input = 1;
    char const* const file_name = argv[1];
    FILE* file = fopen(file_name, "r");
    char line[4096];
    fgets(line, sizeof(line), file);
    fclose(file);
    int intcode[4096];
    int length = intcode_from_csv_line(line, intcode);
    // print_intcode(length, intcode);
    int status = process_intcode(length, intcode, input);
    // print_intcode(length, intcode);
    return status;
}

void print_intcode(int length, int *intcode)
{
    for (int i = 0; i < length; i++) {
        printf("%d ", intcode[i]);
    }
    printf("\n");
}

int intcode_from_csv_line(char *line, int *intcode)
{
    char *pointer;
    int ct = 0;
    pointer = strtok(line, ",");
    while (pointer != NULL) {
        intcode[ct] = atoi(pointer);
        pointer = strtok(NULL, ",");
        ct = ct + 1;
    }
    return ct;
}

int process_intcode(int length, int *intcode, int input)
{
    int opcode;
    int param_mode_one;
    int param_mode_two;
    int param_one;
    int param_two;
    int i = 0;
    while (intcode[i] != 99) {
        opcode = intcode[i];
        param_mode_two = opcode / 1000;
        // printf("param mode 2: %d\n", param_mode_two);
        opcode = opcode - param_mode_two * 1000;
        param_mode_one = opcode / 100;
        // printf("param mode 1: %d\n", param_mode_one);
        opcode = opcode - param_mode_one * 100;
        if (param_mode_one == 0) {
            param_one = intcode[intcode[i+1]];
        } else {
            param_one = intcode[i+1];
        }
        if (param_mode_two == 0) {
            param_two = intcode[intcode[i+2]];
        } else {
            param_two = intcode[i+2];
        }
        if (opcode == 1) {
            intcode[intcode[i+3]] = param_one + param_two;
            i = i + 4;
        } else if (opcode == 2) {
            intcode[intcode[i+3]] = param_one * param_two;
            i = i + 4;
        } else if (opcode == 3) {
            intcode[intcode[i+1]] = input;
            i = i + 2;
        } else if (opcode == 4) {
            input = param_one;
            printf("%d\n", input);
            i = i + 2;
        } else {
            printf("something went wrong\n");
            return 1;
        }
    }
    printf("done\n");
    return 0;
}
