typedef struct Chemical {
    char name[5];
    int output_quantity;
    int num_inputs;
    int* input_quantities;
    struct Chemical** input_chemicals;
    int cache;
} Chemical;
void chemical_from_line(char *line, int *num_chemicals, Chemical *chemicals);
Chemical* find_chemical_by_name(char *name, int *num_chemicals, Chemical *chemicals);
void print_chemical(Chemical* chemical);
void ask_for_chemical(Chemical *chemical, int amount);
// int count_required_ore(char *name, int number, int *num_chemicals, Chemical *chemicals);
// int count_leftover_ore(char *name, int *num_chemicals, Chemical *chemicals);
