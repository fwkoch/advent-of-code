#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main()
{
    int minimum = 307237;
    int maximum = 769058;
    int valid = 0;
    for (int input=minimum; input<=maximum; input++) {
        if (validate(input) == 0) {
            printf("valid: %d\n", input);
            valid++;
        }
    }
    printf("Total valid: %d\n", valid);
}

int validate(int input) {
    int a = input / 100000;
    input = input - a * 100000;
    int b = input / 10000;
    input = input - b * 10000;
    int c = input / 1000;
    input = input - c * 1000;
    int d = input / 100;
    input = input - d * 100;
    int e = input / 10;
    int f = input - e * 10;
    // printf("%d %d %d %d %d %d\n", a, b, c, d, e, f);
    if (a > b || b > c || c > d || d > e || e > f) {
        return 1;
    }
    if (a == b || b == c || c == d || d == e || e == f) {
        return 0;
    }
    return 1;
}
