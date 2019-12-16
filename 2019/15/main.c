#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "main.h"

int main(int argc, char* argv[])
{
    char* file_name = argv[1];
    long intcode[8192] = {};
    intcode_from_csv_line(file_name, intcode);

    long* pointer = &intcode[0];
    int relative_base = 0;

    int screen[8192];
    int x = 0;
    int y = 0;
    screen[0] = 0;
    screen[1] = 0;
    screen[2] = 1;
    int index = 1;
    int dir, status, xnext, ynext;
    char dir_input;
    printf("Autoplay? (y/n): ");
    char autoplay;
    scanf(" %c", &autoplay);

    if (autoplay == 'n') {
        while (1) {
            refresh(index, screen, x, y);
            scanf(" %c", &dir_input);
            dir = dir_input - '0';
            if (dir < 1 || dir > 4) {
                printf("Bad input: %c\n", dir_input);
                continue;
            }
            if (dir == 1) {
                xnext = x;
                ynext = y+1;
            } else if (dir == 2) {
                xnext = x;
                ynext = y-1;
            } else if (dir == 3) {
                xnext = x-1;
                ynext = y;
            } else {
                xnext = x+1;
                ynext = y;
            }
            status = process_intcode(intcode, &pointer, &relative_base, &dir);
            if (get_index(xnext, ynext, index, screen) == -1) {
                screen[index*3] = xnext;
                screen[index*3+1] = ynext;
                screen[index*3+2] = status;
                index++;
            }
            if (status != 0) {
                x = xnext;
                y = ynext;
            }
        }
    } else {
        explore(&index, screen, x, y, 0, intcode, &pointer, &relative_base);
        int oxygen_system_index = -1;
        for (int i = 0; i < index; i++) {
            if (screen[i*3+2] == 2) {
                oxygen_system_index = i;
            }
        }
        if (oxygen_system_index == -1) {
            printf("No oxygen system. :(\n");
            return 1;
        }
        int time = oxygenate(index, screen, screen[oxygen_system_index*3], screen[oxygen_system_index*3+1]);
        printf("Total time: %d minutes\n", time-1);
    }
    return 0;
}

void explore(int* index, int *screen, int x, int y, int home, long *intcode, long **pointer, int *relative_base)
{
    usleep(10000);
    refresh(*index, screen, x, y);
    printf("%d\n", *index);
    int xnext, ynext, back, status;
    for (int i = 1; i < 5; i++) {
        if (i == home) {
            continue;
        }
        if (i == 1) {
            xnext = x;
            ynext = y+1;
            back = 2;
        } else if (i == 3) {
            xnext = x-1;
            ynext = y;
            back = 4;
        } else if (i == 2) {
            xnext = x;
            ynext = y-1;
            back = 1;
        } else {
            xnext = x+1;
            ynext = y;
            back = 3;
        }
        status = process_intcode(intcode, pointer, relative_base, &i);
        if (get_index(xnext, ynext, *index, screen) == -1) {
            screen[*index*3] = xnext;
            screen[*index*3+1] = ynext;
            screen[*index*3+2] = status;
            *index = *index + 1;
        }
        if (status > 0) {
            explore(index, screen, xnext, ynext, back, intcode, pointer, relative_base);
            status = process_intcode(intcode, pointer, relative_base, &back);
        }
    }
}

int oxygenate(int index, int *screen, int x, int y)
{
    int oxy_index = get_index(x, y, index, screen);
    if (oxy_index == -1) {
        return 0;
    }
    if (screen[oxy_index*3+2] <= 0) {
        return 0;
    }
    usleep(10000);
    refresh(index, screen, 1000, 1000);
    screen[get_index(x, y, index, screen)*3+2] = -1;
    int max_time = oxygenate(index, screen, x, y+1);
    int time = oxygenate(index, screen, x-1, y);
    if (time > max_time) {
        max_time = time;
    }
    time = oxygenate(index, screen, x, y-1);
    if (time > max_time) {
        max_time = time;
    }
    time = oxygenate(index, screen, x+1, y);
    if (time > max_time) {
        max_time = time;
    }
    return max_time+1;
}

void print_array(int length, int *array)
{
    for (int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void refresh(int length, int *screen, int xdroid, int ydroid)
{
    int x, y, status;
    int xrange[2] = {-5, 5};
    int yrange[2] = {-5, 5};

    for (int i = 0; i < length; i++) {
        x = screen[3*i];
        y = screen[3*i + 1];
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
    }
    for (int j = yrange[1]; j >= yrange[0]; j--) {
        for (int i = xrange[0]; i <= xrange[1]; i++) {
            if (i == xdroid && j == ydroid) {
                printf("&");
                continue;
            }
            status = get_status(i, j, length, screen);
            if (status == 0) {
                printf("█");
            } else if (status == 1) {
                printf(" ");
            } else if (status == 2) {
                printf("X");
            } else if (status == -1) {
                printf("O");
            } else{
                printf("+");
            }
        }
        printf("\n");
    }
}

int get_index(int x, int y, int length, int *screen)
{
    for (int i = 0; i < length; i++) {
        if (x == screen[i*3] && y == screen[i*3+1]) {
            return i;
        }
    }
    return -1;
}

int get_status(int x, int y, int length, int *screen)
{
    int index = get_index(x, y, length, screen);
    if (index == -1) {
        return -2;
    }
    return screen[index*3+2];
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

int process_intcode(long *intcode, long **pointer, int *rb_pointer, int *input)
{
    int opcode, val;
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
            if (param_mode_one == 0) {
                intcode[*(*pointer+1)] = *input;
            } else {
                intcode[*rb_pointer + *(*pointer+1)] = *input;
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
