int intcode_from_csv_line(char* file_name, long *intcode);
void print_array(int length, long *array);
int process_intcode(long *intcode, long **pointer, int *rb_pointer, long input);
int get_x(int x, char facing, int input);
int get_y(int y, char facing, int input);
char get_facing(char facing, int input);
int get_index(int x, int y, int length, int *xlocs, int *ylocs, int *color);
void print_array_2d(int length, int *xlocs, int *ylocs, int *color);
