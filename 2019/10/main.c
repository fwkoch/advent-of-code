#include <math.h>
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
    int num_visible;
    int max_visible = 0;
    int max_xcoord;
    int max_ycoord;
    int blasted[1024] = {};
    for (int i = 0; i < total; i++) {
        int visible[1024] = {};
        num_visible = count_visible_asteroids(total, xcoord[i], ycoord[i], xcoord, ycoord, visible, blasted);
        if (num_visible > max_visible) {
            max_visible = num_visible;
            max_xcoord = xcoord[i];
            max_ycoord = ycoord[i];
        }
    }
    printf("\nBest is (%d, %d) with %d other asteroids detected\n", max_xcoord, max_ycoord, max_visible);

    int visible[1024] = {};
    num_visible = count_visible_asteroids(total, max_xcoord, max_ycoord, xcoord, ycoord, visible, blasted);
    int laser_quadrant = 2;
    float laser_angle = 0;
    int blast_number = 1;
    float next_angle;
    int next_angle_index, xrel, yrel;
    while (num_visible > 0) {
        // printf("%d asteroids visible\n", num_visible);
        while (num_visible > 0) {
            next_angle = -1;
            for (int i = 0; i < total; i++) {
                // printf("Checking (%d, %d)\n", xcoord[i], ycoord[i]);
                xrel = xcoord[i]-max_xcoord;
                yrel = max_ycoord-ycoord[i];
                if (visible[i] == 0 || blasted[i] > 0) {
                    // printf("Not visible or already blasted\n");
                    continue;
                }
                // printf("   laser quad: %d\n", laser_quadrant);
                // printf("asteroid quad: %d\n", quadrant(xrel, yrel));
                if (quadrant(xrel, yrel) != laser_quadrant) {
                    // printf("Wrong quadrant\n");
                    continue;
                }
                if (laser_quadrant % 2 == 0) {
                    printf("blast #%d: (%d, %d)\n", blast_number, xcoord[i], ycoord[i]);
                    blasted[i] = blast_number;
                    blast_number++;
                    num_visible--;
                    break;
                }
                float angle = get_relative_angle(laser_quadrant, (float)xrel, (float)yrel);
                // printf("asteroid at angle %f compared to laser angle %f\n", angle, laser_angle);
                if (angle > laser_angle && (angle < next_angle || next_angle < 0)) {
                    next_angle = angle;
                    next_angle_index = i;
                    // printf("found better angle: %f\n", next_angle);
                }
            }
            if (next_angle > 0) {
                printf("blast #%d: (%d, %d)\n", blast_number,  xcoord[next_angle_index], ycoord[next_angle_index]);
                blasted[next_angle_index] = blast_number;
                blast_number++;
                num_visible--;
                laser_angle = next_angle;
                next_angle = -1;
            } else {
                laser_quadrant++;
                if (laser_quadrant == 9) {
                    laser_quadrant = 1;
                }
                laser_angle = 0;
            }
        }
        num_visible = count_visible_asteroids(total, max_xcoord, max_ycoord, xcoord, ycoord, visible, blasted);
    }

    return 0;
}

float get_relative_angle(int quad, float x, float y)
{
    if (quad == 3 || quad == 7) {
        return fabsf(x)/fabsf(y);
    }
    return fabsf(y)/fabsf(x);
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

int count_visible_asteroids(int length, int x, int y, int *xcoord, int *ycoord, int *visible, int *blasted)
{
    int xdiff[length];
    int ydiff[length];
    for (int i = 0; i < length; i++) {
        if (blasted[i] > 0) {
            xdiff[i] = 0;
            ydiff[i] = 0;
        } else {
            xdiff[i] = xcoord[i] - x;
            ydiff[i] = ycoord[i] - y;
        }
    }
    int is_visible;
    int total = 0;
    // For each asteroid relative to input asteroid:
    for (int i = 0; i < length; i++) {
        // Don't count the input asteroid
        if (xdiff[i] == 0 && ydiff[i] == 0) {
            continue;
        }
        // Assume it is visible:
        is_visible = 1;
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
                is_visible = 0;
                break;
            }
        }
        visible[i] = is_visible;
        total += is_visible;
    }
    return total;
}

int quadrant(int x, int y)
{
    if (x == 0 && y == 0) {
        return 0;
    }
    if (x < 0 && y > 0) {
        return 1;
    }
    if (x == 0 && y > 0) {
        return 2;
    }
    if (x > 0 && y > 0) {
        return 3;
    }
    if (x > 0 && y == 0) {
        return 4;
    }
    if (x > 0 && y < 0) {
        return 5;
    }
    if (x == 0 && y < 0) {
        return 6;
    }
    if (x < 0 && y < 0) {
        return 7;
    }
    // if (x < 0 && y == 0)
    return 8;
}
