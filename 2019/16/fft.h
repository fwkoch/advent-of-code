int signal_from_line(char *line, int *signal);
void generate_phase(int length, int index, int *phase);
void array_multiply(int length, int *input_a, int *input_b, int *output);
int get_ones_place(int value);
int array_sum(int length, int *input);
int fft_digit(int length, int index, int *input);
void fft(int length, int *input, int *output);
void repeat_fft(int repeats, int length, int *input, int *output);
void print_digits(int amount, int *input);
