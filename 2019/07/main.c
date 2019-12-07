#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char* argv[])
{
    int phases[5] = {1, 2, 3, 4, 5};
    int phase_settings[120*5] = {};
    int* pointer = &phase_settings[0];
    permute(phases, 0, 4, pointer);
    for (int i = 0; i <600; i++) {
        phase_settings[i] = phase_settings[i] - 1;
    }
    print_array(600, phase_settings);
    int max_thruster_signal = 0;
    for (int i = 0; i < 120; i++) {
        char const* const file_name = argv[1];
        FILE* file = fopen(file_name, "r");
        char line[4096];
        fgets(line, sizeof(line), file);
        fclose(file);
        int intcode[4096];
        int length = intcode_from_csv_line(line, intcode);
        // print_array(length, intcode);
        // printf("%d ", phase_settings[i * 5 + 0]);
        // printf("%d ", phase_settings[i * 5 + 1]);
        // printf("%d ", phase_settings[i * 5 + 2]);
        // printf("%d ", phase_settings[i * 5 + 3]);
        // printf("%d\n", phase_settings[i * 5 + 4]);
        int amp_input = 0;
        amp_input = process_intcode(length, intcode, phase_settings[i * 5 + 0], amp_input);
        amp_input = process_intcode(length, intcode, phase_settings[i * 5 + 1], amp_input);
        amp_input = process_intcode(length, intcode, phase_settings[i * 5 + 2], amp_input);
        amp_input = process_intcode(length, intcode, phase_settings[i * 5 + 3], amp_input);
        amp_input = process_intcode(length, intcode, phase_settings[i * 5 + 4], amp_input);
        if (amp_input > max_thruster_signal) {
            max_thruster_signal = amp_input;
            printf("New maximum found: %d", max_thruster_signal);
            printf(" --- Phase setting: ");
            printf("%d ", phase_settings[i * 5 + 0]);
            printf("%d ", phase_settings[i * 5 + 1]);
            printf("%d ", phase_settings[i * 5 + 2]);
            printf("%d ", phase_settings[i * 5 + 3]);
            printf("%d\n", phase_settings[i * 5 + 4]);
        }
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

int process_intcode(int length, int *intcode, int phase_input, int amp_input)
{
    // printf("inputs: %d, %d\n", phase_input, amp_input);
    int input = phase_input;
    int opcode;
    int param_mode_one;
    int param_mode_two;
    int param_one;
    int param_two;
    int i = 0;
    int output;
    while (intcode[i] != 99) {
        opcode = intcode[i];
        param_mode_two = opcode / 1000;
        opcode = opcode - param_mode_two * 1000;
        param_mode_one = opcode / 100;
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
            input = amp_input;
            i = i + 2;
        } else if (opcode == 4) {
            output = param_one;
            // printf("%d\n", output);
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
                intcode[intcode[i+3]] = 1;
            } else {
                intcode[intcode[i+3]] = 0;
            }
            i = i + 4;
        } else if (opcode == 8) {
            if (param_one == param_two) {
                intcode[intcode[i+3]] = 1;
            } else {
                intcode[intcode[i+3]] = 0;
            }
            i = i + 4;
        } else {
            printf("something went wrong\n");
            return -1;
        }
    }
    return output;
}

void swap(int* x, int* y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

void permute(int* array, int start, int end, int* pointer)
{
    if (start == end){
        // print_array(5, array);
        int i = 0;
        while (pointer[i] > 0) {
            i++;
        }
        pointer[i+0] = array[0];
        pointer[i+1] = array[1];
        pointer[i+2] = array[2];
        pointer[i+3] = array[3];
        pointer[i+4] = array[4];
    } else {
        for (int i = start; i <= end; i++) {
            swap(array + start, array + i);
            permute(array, start + 1, end, pointer);
            swap(array + start, array + i);
        }
    }


}
