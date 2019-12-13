int intcode_from_csv_line(char* file_name, long *intcode);
void print_array(int length, int *array);
int process_intcode(long *intcode, long **pointer, int *rb_pointer, int **input);
void refresh(int *index, int *outputs, int *score);
int get_index(int x, int y, int length, int *xvals, int *yvals);
int get_id(int x, int y, int length, int *xvals, int *yvals, int *idvals);
