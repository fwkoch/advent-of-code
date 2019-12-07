#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char* argv[])
{
    int max_thruster_signal = 0;

    // Iterate through all possible permutations of phase input
    int phases[5] = {5, 6, 7, 8, 9};
    int phase_settings[120*5] = {};
    int* pointer = &phase_settings[0];
    permute(phases, 0, 4, pointer);
    for (int i = 0; i < 120; i++) {
        // Read intcode
        char const* const file_name = argv[1];
        FILE* file = fopen(file_name, "r");
        char line[4096];
        fgets(line, sizeof(line), file);
        fclose(file);
        int intcode[4096];
        int length = intcode_from_csv_line(line, intcode);
        // Copy intcode into 5 separate amplifiers
        int intcodes[5][length];
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < length; k++) {
                intcodes[j][k] = intcode[k];
            }
        }
        int pointers[5] = {};
        int amp_input = 0;
        // int iter = 0;
        // printf("Iteration %d\n", iter);

        // Initially process intcodes with two inputs
        for (int j = 0; j < 5; j++) {
            // printf("Amplifier %d -- ", j);
            amp_input = process_intcode(length, j, pointers, intcodes[j], phase_settings[i * 5 + j], amp_input);
            // printf("\n");
        }
        // Continue to pass outputs as inputs until amplifiers stop
        while (amp_input >= 0) {
            // iter++;
            // printf("Iteration %d\n", iter);
            for (int j = 0; j < 5; j++) {
                // printf("Amplifier %d -- ", j);
                amp_input = process_intcode(length, j, pointers, intcodes[j], amp_input, 0);
                // printf("\n");
                if (j == 4 && amp_input > max_thruster_signal) {
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

int process_intcode(int length, int pi, int *pointers, int *intcode, int input_one, int input_two)
{
    int input = input_one;
    int opcode;
    int param_mode_one;
    int param_mode_two;
    int param_one;
    int param_two;
    int i = pointers[pi];
    // printf("start i: %d -- ", i);
    int output = -1;
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
            // printf("input: %d -- ", input);
            intcode[intcode[i+1]] = input;
            input = input_two;
            i = i + 2;
        } else if (opcode == 4) {
            output = param_one;
            // printf("output: %d -- ", output);
            i = i + 2;
            // printf("end i: %d -- ", i);
            pointers[pi] = i;
            return output;
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
            printf("something went wrong");
            return -1;
        }
    }
    // printf("Done");
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
