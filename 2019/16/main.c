#include <stdio.h>
#include <stdlib.h>
#include "fft.h"

int main(int argc, char* argv[])
{
    char* file_name = argv[1];
    char line[1024];
    FILE* file = fopen(file_name, "r");
    fgets(line, sizeof(line), file);
    fclose(file);
    int input[1024];
    int length = signal_from_line(line, input);
    int *signal = malloc(length*10000*sizeof(int));
    for (int i = 0; i < 10000; i++) {
        for (int j = 0; j < length; j++) {
            signal[i*length + j] = input[j];
        }
    }
    int offset = get_digits(0, 7, input);
    printf("offset: %d\n", offset);
    print_digits(offset, 100, signal);
    int *output = malloc(length*10000*sizeof(int));
    fft(length*10000, signal, output);
    print_digits(0, 100, output);
    // int repeats = 100;
    // repeat_fft(repeats, length*10000, signal, output);
    printf("output: %d\n", get_digits(offset, 8, output));
    return 0;
}
