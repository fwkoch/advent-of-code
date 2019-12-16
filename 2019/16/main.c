#include <stdio.h>
#include "fft.h"

int main(int argc, char* argv[])
{
    char* file_name = argv[1];
    char line[1024];
    FILE* file = fopen(file_name, "r");
    fgets(line, sizeof(line), file);
    fclose(file);
    int signal[1024];
    int length = signal_from_line(line, signal);
    int output[1024];
    int repeats = 100;
    repeat_fft(repeats, length, signal, output);
    print_digits(8, output);
    return 0;
}
