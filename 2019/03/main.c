#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char* argv[])
{
    char const* const file_name = argv[1];
    FILE* file = fopen(file_name, "r");
    char line[2048];
    fgets(line, sizeof(line), file);
    // printf("%s", line);
    int xvals[1024];
    int yvals[1024];
    int moves = extract_xy_moves(line, xvals, yvals);
    int steps = absolute_sum(moves, xvals) + absolute_sum(moves, yvals);
    // printf("steps: %d\n", steps);
    int xlocations[steps+1];
    int ylocations[steps+1];
    set_xy_locations(moves, xvals, yvals, xlocations, ylocations);
    fgets(line, sizeof(line), file);
    fclose(file);
    // printf("%s", line);
    moves = extract_xy_moves(line, xvals, yvals);
    int distance = compare_xy_locations(moves, xvals, yvals, steps+1, xlocations, ylocations);
    printf("Distance: %d\n", distance);
}

int extract_xy_moves(char *input, int *xvals, int *yvals)
{
    // printf("Length: %ld\n", strlen(input));
    int input_index = 0;
    int multiplier;
    int vals_index = 0;
    int * value_pointer;
    int * zero_pointer;
    while (input_index < strlen(input) - 1) {
        // printf("RLUD? %c\n", input[input_index]);
        if (strncmp(&input[input_index], "R", 1) == 0) {
            multiplier = 1;
            value_pointer = &xvals[vals_index];
            zero_pointer = &yvals[vals_index];
        } else if (strncmp(&input[input_index], "U", 1) == 0) {
            multiplier = 1;
            value_pointer = &yvals[vals_index];
            zero_pointer = &xvals[vals_index];
        } else if (strncmp(&input[input_index], "L", 1) == 0) {
            multiplier = -1;
            value_pointer = &xvals[vals_index];
            zero_pointer = &yvals[vals_index];
        } else  {
            multiplier = -1;
            value_pointer = &yvals[vals_index];
            zero_pointer = &xvals[vals_index];
        }
        input_index++;
        int value = 0;
        while (input_index < strlen(input) - 1 && strncmp(&input[input_index], ",", 1) != 0) {
            // printf("value: %c\n", input[input_index]);
            value = value * 10 + (input[input_index] - '0');
            input_index++;
        }
        value = multiplier * value;
        // printf("final value: %d\n", value);
        *value_pointer = value;
        *zero_pointer = 0;
        input_index++;
        vals_index++;
    }
    // printf("length: %d\n", vals_index);
    // printf("xvals: ");
    // for (int i = 0; i < vals_index; i++) {
    //     printf("%d ", xvals[i]);
    // }
    // printf("\n");
    // printf("yvals: ");
    // for (int i = 0; i < vals_index; i++) {
    //     printf("%d ", yvals[i]);
    // }
    // printf("\n");
    return vals_index;
}

int absolute_sum(int length, int *array)
{
    int result = 0;
    for (int i=0; i<length; i++) {
        result += abs(array[i]);
    }
    return result;
}

void set_xy_locations(int moves, int *xmoves, int *ymoves, int *xlocs, int *ylocs)
{
    // printf("moves: %d\n", moves);
    xlocs[0] = 0;
    ylocs[0] = 0;
    int index = 1;
    int num_steps;
    int* moves_loc;
    int* still_loc;
    for (int i=0; i<moves; i++) {
        if (xmoves[i] != 0) {
            // printf("x steps: %d\n", xmoves[i]);
            num_steps = xmoves[i];
            moves_loc = xlocs;
            still_loc = ylocs;
        } else {
            // printf("y steps: %d\n", ymoves[i]);
            num_steps = ymoves[i];
            moves_loc = ylocs;
            still_loc = xlocs;
        }
        for (int j=0; j<abs(num_steps); j++) {
            moves_loc[index] = moves_loc[index-1] + num_steps / abs(num_steps);
            still_loc[index] = still_loc[index-1];
            index++;
        }
    }
    // printf("length: %d\n", index);
    // printf("xlocs: ");
    // for (int i = 0; i < index; i++) {
    //     printf("%d ", xlocs[i]);
    // }
    // printf("\n");
    // printf("ylocs: ");
    // for (int i = 0; i < index; i++) {
    //     printf("%d ", ylocs[i]);
    // }
    // printf("\n");
}

int compare_xy_locations(int moves, int *xmoves, int *ymoves, int locations, int *xlocs, int *ylocs)
{
    // printf("moves: %d\n", moves);
    int xloc = 0;
    int yloc = 0;
    int index = 1;
    int num_steps;
    int* moves_loc;
    int* still_loc;
    int distance = 0;
    int travel = 0;
    for (int i=0; i<moves; i++) {
        if (xmoves[i] != 0) {
            // printf("x steps: %d\n", xmoves[i]);
            num_steps = xmoves[i];
            moves_loc = &xloc;
            still_loc = &yloc;
        } else {
            // printf("y steps: %d\n", ymoves[i]);
            num_steps = ymoves[i];
            moves_loc = &yloc;
            still_loc = &xloc;
        }
        for (int j=0; j<abs(num_steps); j++) {
            *moves_loc += num_steps / abs(num_steps);
            index++;
            travel++;
            for (int k=0; k<locations; k++) {
                if (xlocs[k] == xloc && ylocs[k] == yloc) {
                    printf("Meet at (%d, %d) - travel distance of %d", xloc, yloc, k+travel);
                    if (distance == 0 || distance > k+travel) {
                        distance = k+travel;
                        printf(" - new minimum!\n");
                    } else {
                        printf("\n");
                    }
                }
            }
        }
    }
    return distance;
}
