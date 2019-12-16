#include <stdio.h>
#include <string.h>
#include "fft.h"

int signal_from_line(char *line, int *signal)
{
    int i = 0;
    while (line[i] != '\n') {
        signal[i] = line[i] - '0';
        i++;
    }
    return i;
}

void generate_phase(int length, int index, int *phase)
{
    int j;
    for (int i = 0; i < length; i++) {
        j = (i+1)/(index+1);
        if (j%4 == 0 || j%4 == 2) {
            phase[i] = 0;
        } else if (j%4 == 1) {
            phase[i] = 1;
        } else {
            phase[i] = -1;
        }
    }
}

void array_multiply(int length, int *input_a, int *input_b, int *output)
{
    for (int i = 0; i < length; i++) {
        output[i] = input_a[i] * input_b[i];
    }
}

int array_sum(int length, int *input)
{
    int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += input[i];
    }
    return sum;
}

int get_ones_place(int value)
{
    if (value < 0) {
        value = -value;
    }
    return value - (value / 10) * 10;
}

int fft_digit(int length, int index, int *input)
{
    int j, sum = 0;
    for (int i = 0; i < length; i++) {
        j = (i+1)/(index+1);
        if (j%4 == 0 || j%4 == 2) {
            continue;
        } else if (j%4 == 1) {
            sum += input[i%length];
        } else {
            sum -= input[i%length];
        }
    }if (sum < 0) {
        sum = -sum;
    }
    return sum - (sum / 10) * 10;

}

void fft(int length, int *input, int *output)
{
    for (int i = 0; i < length; i++) {
        output[i] = fft_digit(length, i, input);
    }
}

void repeat_fft(int repeats, int length, int *input, int *output)
{
    int temp[length];
    memcpy(temp, input, length * sizeof(int));
    for (int i = 0; i < repeats; i++) {
        if (i%2 == 0) {
            fft(length, temp, output);
        } else {
            fft(length, output, temp);
        }
    }
    if (repeats%2 == 0) {
        memcpy(output, temp, length * sizeof(int));
    }
}

int get_digits(int start, int amount, int *input)
{
    int digits = 0;
    for (int i = 0; i < amount; i++) {
        digits = digits * 10 + input[i+start];
    }
    return digits;
}

void print_digits(int start, int amount, int *input)
{
    for (int i = 0; i < amount; i++) {
        printf("%d", input[i+start]);
    }
    printf("\n");
}
