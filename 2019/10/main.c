#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char* argv[])
{
    char* file_name = argv[1];
    int xcoord[1024] = {};
    int ycoord[1024] = {};
    int total = get_asteroids(file_name, xcoord, ycoord);
    int visible[total];
    int max_visible = 0;
    int max_xcoord;
    int max_ycoord;
    for (int i = 0; i < total; i++) {
        visible[i] = count_visible_asteroids(total, xcoord[i], ycoord[i], xcoord, ycoord);
        if (visible[i] > max_visible) {
            max_visible = visible[i];
            max_xcoord = xcoord[i];
            max_ycoord = ycoord[i];
        }
    }
    printf("\nBest is (%d, %d) with %d other asteroids detected\n", max_xcoord, max_ycoord, max_visible);
    return 0;
}

void print_array(int length, int *array)
{
    for (int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int get_asteroids(char *file_name, int *xcoord, int *ycoord)
{
    FILE* file = fopen(file_name, "r");
    int x = 0;
    int y = 0;
    int ct = 0;
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            x = 0;
            y++;
        } else {
            if (c == '#') {
                xcoord[ct] = x;
                ycoord[ct] = y;
                ct++;
            }
            x++;
        }
    }
    fclose(file);
    return ct;
}

int count_visible_asteroids(int length, int x, int y, int *xcoord, int *ycoord)
{
    int xdiff[length];
    int ydiff[length];
    for (int i = 0; i < length; i++) {
        xdiff[i] = xcoord[i] - x;
        ydiff[i] = ycoord[i] - y;
    }
    int visible;
    int total = 0;
    // For each asteroid relative to input asteroid:
    for (int i = 0; i < length; i++) {
        // Don't count the input asteroid
        if (xdiff[i] == 0 && ydiff[i] == 0) {
            continue;
        }
        // Assume it is visible:
        visible = 1;
        for (int j = 0; j < length; j++) {
            // it doesn't block itself
            if (i == j) {
                continue;
            }
            // it isn't blocked by asteroids in a different quadrant
            if (quadrant(xdiff[i], ydiff[i]) != quadrant(xdiff[j], ydiff[j])) {
                continue;
            }
            // it isn't blocked by asteroids at different angles
            if (xdiff[i] * ydiff[j] != xdiff[j] * ydiff[i]) {
                continue;
            }
            // it is only blocked if it is at the same angle but further away
            if (abs(xdiff[i]) >= abs(xdiff[j]) && abs(ydiff[i]) >= abs(ydiff[j])) {
                visible = 0;
                break;
            }
        }
        total += visible;
    }
    return total;
}

int quadrant(int x, int y)
{
    if (x == 0 && y == 0) {
        return 0;
    }
    if (x > 0 && y > 0) {
        return 1;
    }
    if (x == 0 && y > 0) {
        return 2;
    }
    if (x < 0 && y > 0) {
        return 3;
    }
    if (x < 0 && y == 0) {
        return 4;
    }
    if (x < 0 && y < 0) {
        return 5;
    }
    if (x == 0 && y < 0) {
        return 6;
    }
    if (x > 0 && y < 0) {
        return 7;
    }
    // if (x > 0 && y == 0)
    return 8;
}
