#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fitness.h"
#include "do_iterations.h"

int main(int argc, char **argv) {
    const float
            max_pos = 31,
            min_pos = 5;

    int
        nofp = atoi(argv[4]), // Número de partículas.
        nofd = 1, // Número de dimensões de fitness.
        nofi = atoi(argv[5]); // Número de iterações.

    float
            inertia = (float) atof(argv[1]),
            cognitive = (float) atof(argv[2]),
            social = (float) atof(argv[3]),
            (*fitness_ptr)(float *);

    srand(time(0));

    fitness_ptr = &fitness;

    do_iterations(fitness_ptr, nofd, min_pos, max_pos, inertia, social, cognitive, nofp, nofi);

    printf("Program execution ended.\n");

    return 0;
}
