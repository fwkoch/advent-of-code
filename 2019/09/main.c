#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char* argv[])
{
    char* file_name = argv[1];
    long intcode[4096] = {};
    int length = intcode_from_csv_line(file_name, intcode);
    long output = process_intcode(length, intcode, 1);
    printf("%ld\n", output);
    return 0;
}

void print_array(int length, long *array)
{
    for (int i = 0; i < length; i++) {
        printf("%ld ", array[i]);
    }
    printf("\n");
}

int intcode_from_csv_line(char* file_name, long *intcode)
{
    FILE* file = fopen(file_name, "r");
    long value = 0;
    int ct = 0;
    int c;
    int multiplier = 1;
    while ((c = fgetc(file)) != '\n') {
        if (c == ',') {
            intcode[ct] = multiplier * value;
            ct++;
            value = 0;
            multiplier = 1;
        } else if (c == '-') {
            multiplier = -1;
        } else {
            value = (value * 10) + (c - '0');
        }
    }
    intcode[ct] = value;
    ct++;
    fclose(file);
    return ct;
}

long process_intcode(int length, long *intcode, int input)
{
    int opcode;
    int param_mode_one;
    int param_mode_two;
    int param_mode_three;
    long param_one;
    long param_two;
    int i = 0;
    int relative_base = 0;
    int val;
    long output = -1;
    while (intcode[i] != 99) {
        printf("intcode: %ld %ld %ld %ld\n", intcode[i], intcode[i+1], intcode[i+2], intcode[i+3]);
        opcode = intcode[i];
        param_mode_three = opcode / 10000;
        opcode = opcode - param_mode_three * 10000;
        param_mode_two = opcode / 1000;
        opcode = opcode - param_mode_two * 1000;
        param_mode_one = opcode / 100;
        opcode = opcode - param_mode_one * 100;
        if (param_mode_one == 0) {
            param_one = intcode[intcode[i+1]];
        } else if (param_mode_one == 1) {
            param_one = intcode[i+1];
        } else {
            param_one = intcode[relative_base + intcode[i+1]];
        }
        if (opcode == 3 || opcode == 4 || opcode == 9) {
            param_two = 0;
        } else if (param_mode_two == 0) {
            param_two = intcode[intcode[i+2]];
        } else if (param_mode_two == 1) {
            param_two = intcode[i+2];
        } else {
            param_two = intcode[relative_base + intcode[i+2]];
        }
        // printf(" -> %d %ld %ld\n", opcode, param_one, param_two);
        if (opcode == 1) {
            if (param_mode_three == 0) {
                intcode[intcode[i+3]] = param_one + param_two;
            } else {
                intcode[relative_base + intcode[i+3]] = param_one + param_two;
            }
            i = i + 4;
        } else if (opcode == 2) {
            if (param_mode_three == 0) {
                intcode[intcode[i+3]] = param_one * param_two;
            } else {
                intcode[relative_base + intcode[i+3]] = param_one * param_two;
            }
            i = i + 4;
        } else if (opcode == 3) {
            if (param_mode_one == 0) {
                intcode[intcode[i+1]] = input;
            } else {
                intcode[relative_base + intcode[i+1]] = input;
            }
            i = i + 2;
        } else if (opcode == 4) {
            output = param_one;
            // printf("output: %ld\n", output);
            i = i + 2;
        } else if (opcode == 5) {
            if (param_one != 0) {
                i = param_two;
            } else {
                i = i + 3;
            }
        } else if (opcode == 6) {
            if (param_one == 0) {
                i = param_two;
            } else {
                i = i + 3;
            }
        } else if (opcode == 7) {
            if (param_one < param_two) {
                val = 1;
            } else {
                val = 0;
            }
            if (param_mode_three == 0) {
                intcode[intcode[i+3]] = val;
            } else {
                intcode[relative_base + intcode[i+3]] = val;
            }
            i = i + 4;
        } else if (opcode == 8) {
            if (param_one == param_two) {
                val = 1;
            } else {
                val = 0;
            }
            if (param_mode_three == 0) {
                intcode[intcode[i+3]] = val;
            } else {
                intcode[relative_base + intcode[i+3]] = val;
            }
            i = i + 4;
        } else if (opcode == 9) {
            relative_base += param_one;
            i = i + 2;
        } else {
            printf("something went wrong");
            return -1;
        }
    }
    // printf("Done");
    return output;
}
