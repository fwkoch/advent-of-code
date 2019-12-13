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
int process_intcode(long *intcode, long **pointer, int *rb_pointer, int **input);
/** Calculate next paddle move direction based on screen conditions.
 *  Given the state of the screen, calculate input required to move the
 *  paddle towards the ball.
 *  \param[in] screen_length Size of the screen array.
 *  \param[in] screen The memory area with screen information, with
 *                    interleaved x, y, and id values.
 *  \return Output -1 to move paddle left, 1 to move paddle right, and
 *          0 to keep paddle in the same place.
 */
int machine_input(int screen_length, int *screen);
/** Receive next paddle move direction from user input.
 *  To move left, the user must enter 'a' or 'j'. To move right, 'd' or
 *  'l'. To stay in the same place, 's' or 'k'.
 *  \return Output -1 to move paddle left, 1 to move paddle right, and
 *          0 to keep paddle in the same place.
 */
int user_input();
/** Refresh the printed screen.
 *  \param[in] screen_length Size of the screen array.
 *  \param[in] screen The memory area with screen information, with
 *                    interleaved x, y, and id values.
 *  \param[in] score Pointer to the current user's score.
 */
void refresh(int *screen_length, int *screen, int *score);
/** Get the index for given x and y screen values.
 *  \param[in] x X value needing corresponding index.
 *  \param[in] y Y value needing corresponding index.
 *  \param[in] length total number of x and y values.
 *  \param[in] xvals The memory area with x values.
 *  \param[in] yvals The memory area with y values.
 *  \return Index of x/y pair in xvals/yvals, or -1 if x/y pari is not
 *          present in xvals/yvals.
 */
int get_index(int x, int y, int length, int *xvals, int *yvals);
/** Get the ID for given x and y screen values.
 *  \param[in] x X value needing corresponding ID.
 *  \param[in] y Y value needing corresponding ID.
 *  \param[in] length total number of x and y values.
 *  \param[in] xvals The memory area with x values.
 *  \param[in] yvals The memory area with y values.
 *  \param[in] idvals The memory area with ID values.
 *  \return ID from idvals of x/y pair in xvals/yvals.
 */
int get_id(int x, int y, int length, int *xvals, int *yvals, int *idvals);
