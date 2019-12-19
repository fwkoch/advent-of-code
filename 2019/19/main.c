#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "main.h"

int main(int argc, char* argv[])
{
    char* file_name = argv[1];
    long intcode[8192] = {};
    long* pointer;
    int relative_base;

    int output;
    int affected_points = 0;
    for (int x = 0; x < 50; x++) {
        for (int y = 0; y < 50; y++) {
            intcode_from_csv_line(file_name, intcode);
            relative_base = 0;
            pointer = &intcode[0];
            output = process_intcode(intcode, &pointer, &relative_base, x, y);
            if (output == 1) {
                printf("#");
                affected_points++;
            } else {
                printf(".");
            }
            }
        printf("\n");
    }
    printf("Affected points: %d\n", affected_points);
    return 0;
}
int get_index(int x, int y, int length, int *xvals, int *yvals)
{
    for (int i = 0; i < length; i++) {
        if (x == xvals[i] && y == yvals[i]) {
            return i;
        }
    }
    return -1;
}

int get_status(int x, int y, int length, int *xvals, int *yvals, int *status)
{
    int index = get_index(x, y, length, xvals, yvals);
    if (index == -1) {
        return 46;
    }
    return status[index];
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

int process_intcode(long *intcode, long **pointer, int *rb_pointer, int input_x, int input_y)
{
    int opcode, val;
    int param_mode_one, param_mode_two, param_mode_three;
    long param_one, param_two;
    int output = -1;
    int input = input_x;
    int input_count = 0;
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
            param_one = intcode[*rb_pointer + *(*pointer+1)];
        }
        if (opcode == 3 || opcode == 4 || opcode == 9) {
            param_two = 0;
        } else if (param_mode_two == 0) {
            param_two = intcode[*(*pointer+2)];
        } else if (param_mode_two == 1) {
            param_two = *(*pointer+2);
        } else {
            param_two = intcode[*rb_pointer + *(*pointer+2)];
        }
        if (opcode == 1) {
            if (param_mode_three == 0) {
                intcode[*(*pointer+3)] = param_one + param_two;
            } else {
                intcode[*rb_pointer + *(*pointer+3)] = param_one + param_two;
            }
            *pointer += 4;
        } else if (opcode == 2) {
            if (param_mode_three == 0) {
                intcode[*(*pointer+3)] = param_one * param_two;
            } else {
                intcode[*rb_pointer + *(*pointer+3)] = param_one * param_two;
            }
            *pointer += 4;
        } else if (opcode == 3) {
            if (input_count > 1) {
                printf("Too many inputs processed\n");
                return -1;
            }
            if (param_mode_one == 0) {
                // printf("Accessing index: %ld\n", *(*pointer+1));
                intcode[*(*pointer+1)] = input;
            } else {
                // printf("Accessing index: %ld\n", *rb_pointer + *(*pointer+1));
                intcode[*rb_pointer + *(*pointer+1)] = input;
            }
            *pointer += 2;
            input = input_y;
            input_count++;
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
                intcode[*rb_pointer + *(*pointer+3)] = val;
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
                intcode[*rb_pointer + *(*pointer+3)] = val;
            }
            *pointer += 4;
        } else if (opcode == 9) {
            *rb_pointer += param_one;
            *pointer += 2;
        } else {
            printf("something went wrong with intcode...%d\n", opcode);
            return -1;
        }
    }
    *pointer = NULL;
    return output;
}
