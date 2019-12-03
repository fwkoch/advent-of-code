#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char* argv[])
{
    char const* const file_name = argv[1];
    FILE* file = fopen(file_name, "r");
    char line[1024];
    fgets(line, sizeof(line), file);
    fclose(file);
    printf("%s\n", line);
    int intcode[256];
    int length = intcode_from_csv_line(line, intcode);
    print_intcode(length, intcode);
    intcode[1] = 12;
    intcode[2] = 2;
    print_intcode(length, intcode);
    int status = process_intcode(length, intcode);
    print_intcode(length, intcode);
    return status;
}

void print_intcode(int length, int *intcode)
{
    for (int i = 0; i < length; i++) {
        printf("%d ", intcode[i]);
    }
    printf("\n\n");
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

int process_intcode(int length, int *intcode)
{
    int i = 0;
    while (intcode[i] != 99) {
        if (intcode[i] == 1) {
            intcode[intcode[i+3]] = intcode[intcode[i+1]] + intcode[intcode[i+2]];
            i = i + 4;
        } else if (intcode[i] == 2) {
            intcode[intcode[i+3]] = intcode[intcode[i+1]] * intcode[intcode[i+2]];
            i = i + 4;
        } else if (intcode[i] == 99) {
            return 0;
        } else {
            return 1;
        }
    }
    return 1;
}
