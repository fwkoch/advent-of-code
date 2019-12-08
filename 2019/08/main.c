#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char* argv[])
{
    int width = 25;
    int height = 6;
    int layer_length = width * height;
    char const* const file_name = argv[1];
    FILE* file = fopen(file_name, "r");
    int image[16384];
    int ct = 0;
    int c;
    while ((c = fgetc(file)) != '\n') {
        image[ct] = c - '0';
        ct++;
    }
    fclose(file);
    int message[layer_length];
    int num_layers = ct / layer_length;
    for (int i = 0; i < layer_length; i++) {
        int layer_index = 0;
        int layer_value = image[layer_index * layer_length + i];
        while (layer_value == 2) {
            layer_index++;
            if (layer_index >= num_layers) {
                printf("max layers exceeded");
                return 1;
            }
            layer_value = image[layer_index * layer_length + i];
        }
        message[i] = layer_value;
    }
    print_array(width, height, message);
    return 0;
}

int count_number(int number, int length, int *array)
{
    int count = 0;
    for (int i = 0; i < length; i++) {
        if (array[i] == number) {
            count++;
        }
    }
    return count;
}

void print_array(int width, int height, int *array)
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (array[i * width + j] == 0) {
                printf(" ");
            } else {
                printf("#");
            }
        }
        printf("\n");
    }
}
