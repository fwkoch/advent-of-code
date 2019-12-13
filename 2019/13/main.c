#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char* argv[])
{
    char* file_name = argv[1];
    long intcode[8192] = {};
    intcode_from_csv_line(file_name, intcode);

    long* pointer = &intcode[0];
    int relative_base = 0;
    int* input = NULL;

    int index = 0;
    int capacity = 2;
    int* screen = malloc(capacity * sizeof(int));
    int output = process_intcode(intcode, &pointer, &relative_base, &input);

    int score = 0;

    while (pointer != NULL) {
        while (input == NULL) {
            if (index >= capacity) {
                capacity = capacity * 2;
                screen = realloc(screen, capacity * sizeof(int));
            }
            screen[index] = output;
            index++;
            output = process_intcode(intcode, &pointer, &relative_base, &input);
        }
        refresh(&index, screen, &score);
        *input = choose_input(index, screen);
        output = process_intcode(intcode, &pointer, &relative_base, &input);
    }
    return 0;
}

void print_array(int length, int *array)
{
    for (int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int choose_input(int index, int *screen)
{
    char joystick;
    int input;
    do {
        scanf(" %c", &joystick);
        if (joystick == 'a' || joystick == 'j') {
            input = -1;
        } else if (joystick == 'd' || joystick == 'l') {
            input = 1;
        } else if (joystick == 's' || joystick == 'k') {
            input = 0;
        } else {
            input = 2;
        }

    } while (input != -1 && input != 0 && input != 1);
    return input;
}

void refresh(int* index, int *screen, int *score)
{
    int length = *index/3;
    int x, y, id, prev_index;
    int* xvals = malloc(sizeof(int) * length);
    int* yvals = malloc(sizeof(int) * length);
    int* idvals = malloc(sizeof(int) * length);
    int xrange[2] = {screen[0], screen[0]};
    int yrange[2] = {screen[1], screen[1]};

    for (int i = 0; i < length; i++) {
        x = screen[3*i];
        y = screen[3*i + 1];
        id = screen[3*i + 2];
        if (x == -1 && y == 0 && id > 0) {
            *score = id;
            continue;
        }
        prev_index = get_index(x, y, i, xvals, yvals);
        if (prev_index >= 0) {
            screen[3*prev_index + 2] = id;
            idvals[prev_index] = id;
            *index = *index - 3;
            continue;
        }
        xvals[i] = x;
        yvals[i] = y;
        if (x < xrange[0]) {
            xrange[0] = x;
        }
        if (x > xrange[1]) {
            xrange[1] = x;
        }
        if (y < yrange[0]) {
            yrange[0] = y;
        }
        if (y > yrange[1]) {
            yrange[1] = y;
        }
        idvals[i] = id;
    }
    for (int j = yrange[0]; j <= yrange[1]; j++) {
        for (int i = xrange[0]; i <= xrange[1]; i++) {
            id = get_id(i, j, length, xvals, yvals, idvals);
            if (id == 0) {
                printf(" ");
            } else if (id == 1) {
                printf("█");
            } else if (id == 2) {
                printf("▓");
            } else if (id == 3) {
                printf("═");
            } else {
                printf("o");
            }
        }
        if (j == yrange[0]) {
            printf(" SCORE: %d", *score);
        }
        printf("\n");
    }
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

int get_id(int x, int y, int length, int *xvals, int *yvals, int *idvals)
{
    return idvals[get_index(x, y, length, xvals, yvals)];
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

int process_intcode(long *intcode, long **pointer, int *rb_pointer, int **input)
{
    int opcode, val, a;
    int param_mode_one, param_mode_two, param_mode_three;
    long param_one, param_two;
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
            if (*input == NULL) {
                a = 2;
                *input = &a;
                return output;
            }
            if (param_mode_one == 0) {
                intcode[*(*pointer+1)] = **input;
            } else {
                intcode[*rb_pointer + *(*pointer+1)] = **input;
            }
            *pointer += 2;
            *input = NULL;
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
            printf("something went wrong with intcode...\n");
            return -1;
        }
    }
    *pointer = NULL;
    return output;
}
