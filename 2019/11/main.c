#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char* argv[])
{
    char* file_name = argv[1];
    long intcode[4096] = {};
    intcode_from_csv_line(file_name, intcode);
    long* pointer = &intcode[0];

    int xlocs[8192] = {};
    int ylocs[8192] = {};
    int color[8192] = {};
    int x = 0;
    int y = 0;
    int length = 0;
    int index;
    int input = 0;
    char facing = '^';
    while (input >= 0) {
        index = get_index(x, y, length, xlocs, ylocs, color);
        if (index == length) {
            length++;
        }
        input = color[index];
        input = process_intcode(intcode, &pointer, input);
        color[index] = input;
        input = process_intcode(intcode, &pointer, input);
        if (input >= 0) {
            x = get_x(x, facing, input);
            y = get_y(y, facing, input);
            facing = get_facing(facing, input);
            printf("(%d, %d) %c\n", x, y, facing);
        }
    }


    printf("%d\n", length);
    return 0;
}

int get_x(int x, char facing, int input)
{
    if (facing == '<' || facing == '>') {
        return x;
    }
    if ((facing == '^' && input == 0) || (facing == 'v' && input == 1)) {
        return x-1;
    }
    if ((facing == '^' && input == 1) || (facing == 'v' && input == 0)) {
        return x+1;
    }
    printf("Something went wrong with x...\n");
    return 0;
}

int get_y(int y, char facing, int input)
{
    if (facing == '^' || facing == 'v') {
        return y;
    }
    if ((facing == '<' && input == 0) || (facing == '>' && input == 1)) {
        return y-1;
    }
    if ((facing == '<' && input == 1) || (facing == '>' && input == 0)) {
        return y+1;
    }
    printf("Something went wrong with y...\n");
    return 0;
}

char get_facing(char facing, int input)
{
    if ((facing == '^' && input == 0) || (facing == 'v' && input == 1)) {
        return '<';
    }
    if ((facing == '^' && input == 1) || (facing == 'v' && input == 0)) {
        return '>';
    }
    if ((facing == '<' && input == 0) || (facing == '>' && input == 1)) {
        return 'v';
    }
    if ((facing == '<' && input == 1) || (facing == '>' && input == 0)) {
        return '^';
    }
    printf("Something went wrong with facing...\n");
    return 0;
}

int get_index(int x, int y, int length, int *xlocs, int *ylocs, int *color)
{
    for (int i = 0; i < length; i++) {
        if (x == xlocs[i] && y == ylocs[i]) {
            return i;
        }
    }
    xlocs[length] = x;
    ylocs[length] = y;
    color[length] = 0;
    return length;
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

int process_intcode(long *intcode, long **pointer, long input)
{
    int opcode;
    int param_mode_one;
    int param_mode_two;
    int param_mode_three;
    long param_one;
    long param_two;
    int relative_base = 0;
    int val;
    int output = -1;
    while (**pointer != 99) {
        opcode = **pointer;
        param_mode_three = opcode / 10000;
        opcode = opcode - param_mode_three * 10000;
        param_mode_two = opcode / 1000;
        opcode = opcode - param_mode_two * 1000;
        param_mode_one = opcode / 100;
        opcode = opcode - param_mode_one * 100;
        if (param_mode_one == 0) {
            param_one = intcode[*(*pointer+1)];
        } else if (param_mode_one == 1) {
            param_one = *(*pointer+1);
        } else {
            param_one = intcode[relative_base + *(*pointer+1)];
        }
        if (opcode == 3 || opcode == 4 || opcode == 9) {
            param_two = 0;
        } else if (param_mode_two == 0) {
            param_two = intcode[*(*pointer+2)];
        } else if (param_mode_two == 1) {
            param_two = *(*pointer+2);
        } else {
            param_two = intcode[relative_base + *(*pointer+2)];
        }
        if (opcode == 1) {
            if (param_mode_three == 0) {
                intcode[*(*pointer+3)] = param_one + param_two;
            } else {
                intcode[relative_base + *(*pointer+3)] = param_one + param_two;
            }
            *pointer += 4;
        } else if (opcode == 2) {
            if (param_mode_three == 0) {
                intcode[*(*pointer+3)] = param_one * param_two;
            } else {
                intcode[relative_base + *(*pointer+3)] = param_one * param_two;
            }
            *pointer += 4;
        } else if (opcode == 3) {
            if (param_mode_one == 0) {
                intcode[*(*pointer+1)] = input;
            } else {
                intcode[relative_base + *(*pointer+1)] = input;
            }
            *pointer += 2;
        } else if (opcode == 4) {
            output = param_one;
            *pointer += 2;
            return output;
        } else if (opcode == 5) {
            if (param_one != 0) {
                *pointer = &intcode[param_two];
            } else {
                *pointer += 3;
            }
        } else if (opcode == 6) {
            if (param_one == 0) {
                *pointer = &intcode[param_two];
            } else {
                *pointer += 3;
            }
        } else if (opcode == 7) {
            if (param_one < param_two) {
                val = 1;
            } else {
                val = 0;
            }
            if (param_mode_three == 0) {
                intcode[*(*pointer+3)] = val;
            } else {
                intcode[relative_base + *(*pointer+3)] = val;
            }
            *pointer += 4;
        } else if (opcode == 8) {
            if (param_one == param_two) {
                val = 1;
            } else {
                val = 0;
            }
            if (param_mode_three == 0) {
                intcode[*(*pointer+3)] = val;
            } else {
                intcode[relative_base + *(*pointer+3)] = val;
            }
            *pointer += 4;
        } else if (opcode == 9) {
            relative_base += param_one;
            *pointer += 2;
        } else {
            printf("something went wrong with intcode...\n");
            return -1;
        }
    }
    printf("Done\n");
    return output;
}
