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

    long deck_size = 10007;

    long num, end;
    long start = 2019;
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "cut ", 4) == 0) {
            num = num_from_line(&line[4]);
            end = cut(num, deck_size, start);
        } else if (strncmp(line, "deal into new stack", 19) == 0) {
            end = reverse(deck_size, start);
        } else {
            num = num_from_line(&line[20]);
            end = increment(num, deck_size, start);
        }
        start = end;
    }
    fclose(file);
    printf("%ld\n", start);
    return 0;
}

long num_from_line(char *line)
{
    int multiplier = 1;
    int ct = 0;
    if (strncmp(&line[0], "-", 1) == 0) {
        multiplier = -1;
        ct++;
    }
    long value = 0;
    while (strncmp(&line[ct], "\n", 1) != 0) {
        value = value * 10 + (line[ct] - '0');
        ct++;
    }
    return multiplier * value;
}

long cut(long num, long length, long start)
{
    if (num < 0) {
        num = length + num;
    }
    if (start < num) {
        return length - num + start;
    }
    return start - num;
}

long increment(long num, long length, long start)
{
    return (start*num) % length;
}

long reverse(long length, long start)
{
    return length - start - 1;
}
