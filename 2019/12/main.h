typedef struct Moon {
    int position[3];
    int velocity[3];
} Moon;
void print_moons(Moon *moons);
void initialize_moon(char* line, Moon* moon);
void update_velocities(Moon* moons);
void update_dimension_velocities(Moon* moons, int dim);
void update_positions(Moon* moons);
void update_dimension_positions(Moon* moons, int dim);
int potential_energy(Moon* moon);
int kinetic_energy(Moon* moon);
int calculate_energy(Moon* moons);
int compare_moons(Moon* moons, Moon* moons_init, int dim);
long least_common_multiple(long a, long b, long c);
