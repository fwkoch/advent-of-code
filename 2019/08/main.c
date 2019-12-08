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
    print_array(ct, &image[0]);
    int num_layers = ct / layer_length;
    printf("num layers: %d\n", num_layers);
    fclose(file);
    int zeros;
    int fewest_zeros = layer_length + 1;
    int product;
    for (int i = 0; i < num_layers; i++) {
        printf("Layer %d: ", i);
        zeros = count_number(0, layer_length, &image[layer_length * i]);
        printf("Zeros: %d", zeros);
        if (zeros < fewest_zeros) {
            fewest_zeros = zeros;
            int ones = count_number(1, layer_length, &image[layer_length * i]);
            int twos = count_number(2, layer_length, &image[layer_length * i]);
            product = ones * twos;
            printf(" New product: %d", product);
        }
        printf("\n");
    }

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

void print_array(int length, int *array)
{
    for (int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}
