int get_asteroids(char* file_name, int* xcoord, int* ycoord);
void print_array(int length, int *array);
int count_visible_asteroids(int length, int x, int y, int *xcoord, int *ycoord, int *visible, int *blasted);
int quadrant(int x, int y);
float get_relative_angle(int quad, float x, float y);
