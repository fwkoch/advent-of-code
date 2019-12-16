/** Read intcode input from file.
 *  Intcode must be a single-line CSV file.
 *  \param[in] file_name The name of the CSV file.
 *  \param[out] intcode The memory area to write intcode.
 *  \return Total length of intcode array.
 */
int intcode_from_csv_line(char* file_name, long *intcode);
/** Print all values in an array.
 *  Values are space-delimited.
 *  \param[in] length Length of the array.
 *  \param[in] array Pointer to the array in memory.
 */
void print_array(int length, int *array);
/** Execute the intcode computer.
 *  Execution begins from the starting pointer location and runs until
 *  (1) output is produced, (2) input is required, or (3) intcode
 *  execution reaches the stopping condition. For more information on
 *  the details of the intcode computer, see description at
 *  https://adventofcode.com/2019
 *  \param[in] intcode Pointer to intcode array in memory.
 *  \param[in] pointer Pointer to the current position in intcode array.
 *  \param[in] rb_pointer Pointer to the relative base value.
 *  \param[in] input Pointer to the position of the input value.
 *  \return Output value of the intcode execution or -1 if input is
 *          required or execution reaches the stopping condition.
 */
int process_intcode(long *intcode, long **pointer, int *rb_pointer, int *input);
void refresh(int length, int *screen, int xdroid, int ydroid);
int get_index(int x, int y, int length, int *screen);
int get_status(int x, int y, int length, int *screen);
void explore(int *index, int *screen, int x, int y, int home, long *intcode, long **pointer, int *relative_base);
int oxygenate(int index, int *screen, int x, int y);
