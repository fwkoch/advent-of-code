typedef struct Chemical {
    char name[5];
    int output_quantity;
    int num_inputs;
    int* input_quantities;
    struct Chemical** input_chemicals;
    long cache;
} Chemical;
void chemical_from_line(char *line, int *num_chemicals, Chemical *chemicals);
Chemical* find_chemical_by_name(char *name, int *num_chemicals, Chemical *chemicals);
void print_chemical(Chemical* chemical);
int fill_chemical_cache(Chemical *chemical, long amount);
void remove_from_cache(Chemical *chemical, long amount);
