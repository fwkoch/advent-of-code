typedef struct Moon {
    int position[3];
    int velocity[3];
} Moon;
void print_moons(Moon *moons);
void initialize_moon(char* line, Moon* moon);
void update_velocities(Moon* moons);
void update_positions(Moon* moons);
int potential_energy(Moon* moon);
int kinetic_energy(Moon* moon);
int calculate_energy(Moon* moons);
