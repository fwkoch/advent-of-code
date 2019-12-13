int intcode_from_csv_line(char* file_name, long *intcode);
void print_array(int length, int *array);
int process_intcode(long *intcode, long **pointer, int *rb_pointer, long input);
void print_screen(int length, int *outputs);
int get_id(int x, int y, int length, int *xvals, int *yvals, int *idvals);
