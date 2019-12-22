#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char* argv[])
{
    char* file_name = argv[1];
    FILE* file;
    file = fopen(file_name, "r");
    char line[256];

    int deck_size = 10007;
    int input_deck[10007], output_deck[10007];
    for (int i = 0; i < deck_size; i++) {
        input_deck[i] = i;
    }

    int num;
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "cut ", 4) == 0) {
            num = num_from_line(&line[4]);
            cut(num, deck_size, input_deck, output_deck);
        } else if (strncmp(line, "deal into new stack", 19) == 0) {
            reverse(deck_size, input_deck, output_deck);
        } else {
            num = num_from_line(&line[20]);
            increment(num, deck_size, input_deck, output_deck);
        }
        memcpy(input_deck, output_deck, sizeof(int)*deck_size);
    }
    fclose(file);
    for (int i = 0; i < deck_size; i++) {
        if (input_deck[i] == 2019) {
            printf("%d\n", i);
            break;
        }
    }
    return 0;
}

int num_from_line(char *line)
{
    int multiplier = 1;
    int ct = 0;
    if (strncmp(&line[0], "-", 1) == 0) {
        multiplier = -1;
        ct++;
    }
    int value = 0;
    while (strncmp(&line[ct], "\n", 1) != 0) {
        value = value * 10 + (line[ct] - '0');
        ct++;
    }
    return multiplier * value;
}

void cut(int num, int length, int *input_deck, int *output_deck)
{
    if (num < 0) {
        num = length + num;
    }
    for (int i = 0; i < num; i++) {
        output_deck[length-num+i] = input_deck[i];
    }
    for (int i = num; i < length; i++) {
        output_deck[i-num] = input_deck[i];
    }
}

void increment(int num, int length, int *input_deck, int *output_deck)
{
    for (int i = 0; i < length; i++) {
        output_deck[(i*num)%length] = input_deck[i];
    }
}

void reverse(int length, int *input_deck, int *output_deck)
{
    for (int i = 0; i < length; i++) {
        output_deck[length-i-1] = input_deck[i];
    }
}

void print_array(int length, int *array)
{
    for (int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}
