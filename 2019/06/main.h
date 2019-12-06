struct Planet {
    char name[4];
    int direct_orbits;
    struct Planet* direct_orbiters[100];
};
int count_total_orbiters(struct Planet planet);
int count_total_orbits(struct Planet planet);
int count_steps_outward(struct Planet start, char end[3]);
int find_shortest_distance(struct Planet* planets, int num_planets, char node_one[3], char node_two[3]);
