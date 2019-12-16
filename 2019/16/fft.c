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
