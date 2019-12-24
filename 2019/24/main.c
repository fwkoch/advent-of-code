#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "main.h"

int main(int argc, char* argv[])
{
    char* file_name = argv[1];
    FILE* file;
    file = fopen(file_name, "r");
    char c;
    int bugs[50] = {};
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            c = fgetc(file);
            if (c == '#') {
                bugs[i*5+j] = 1;
            }
        }
        fgetc(file);  // newlines
    }
    fclose(file);
    print_array(bugs);

    int index = 0;
    int capacity = 2;
    int* biodivs = malloc(capacity * sizeof(int));
    biodivs[index] = biodiversity(25, &bugs[0]);

    int phase = 0;
    while (last_index_unique(index, biodivs)) {
        index++;
        if (index >= capacity) {
            capacity = capacity * 2;
            biodivs = realloc(biodivs, capacity * sizeof(int));
        }
        if (phase == 0) {
            time_step(&bugs[0], &bugs[25]);
            biodivs[index] = biodiversity(25, &bugs[25]);
            print_array(&bugs[25]);
            phase = 1;
        } else {
            time_step(&bugs[25], &bugs[0]);
            biodivs[index] = biodiversity(25, &bugs[0]);
            print_array(&bugs[0]);
            phase = 0;
        }
    }
    printf("First repeated biodiv: %d\n", biodivs[index]);
    return 0;
}

void print_array(int *array)
{
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++){
            if (array[i*5 + j] == 1) {
                printf("#");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    printf("\n");
}

int biodiversity(int length, int *array)
{
    int biodiv = 0;
    for (int i = 0; i < length; i++) {
        if (array[i]) {
            biodiv += pow(2, i);
        }
    }
    return biodiv;
}

int last_index_unique(int index, int *array)
{
    int value = array[index];
    for (int i = 0; i < index; i++) {
        if (array[i] == value) {
            return 0;
        }
    }
    return 1;
}

void time_step(int *now, int *later)
{
    int bug_sum;
    for (int i = 0; i < 25; i++) {
        bug_sum = 0;
        if (i >= 5) {
            bug_sum += now[i-5];
        }
        if (i < 20) {
            bug_sum += now[i+5];
        }
        if (i % 5 != 0) {
            bug_sum += now[i-1];
        }
        if (i % 5 != 4) {
            bug_sum += now[i+1];
        }
        if (now[i] == 1 && bug_sum != 1) {
            later[i] = 0;
        } else if (now[i] == 0 && (bug_sum == 1 || bug_sum == 2)) {
            later[i] = 1;
        } else {
            later[i] = now[i];
        }
    }
}
