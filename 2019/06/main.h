struct Planet {
    char name[4];
    int direct_orbits;
    struct Planet* direct_orbiters[100];
};
int count_total_orbiters(struct Planet planet);
int count_total_orbits(struct Planet planet);
