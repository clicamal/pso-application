#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct {
    float
            *pos,
            *best_pos,
            *vel;

    float
            r,
            cognitive;
} particle;

typedef struct {
    float
            *g_best_pos,
            rg;

    particle *particles;
} iter_info;

typedef struct {
    iter_info *iters;

    float
            inertia,
            cognitive,
            social,
            *g_best_pos;

} run_info;

float fitness(float *input) {
    float  x = input[0];
    return (12 * powf(x, 5) - 975 * powf(x, 4) + 28000 * powf(x, 3) - 345000 * powf(x, 2) + 1800000 * x) / 1000000;
}

int main(int argc, char **argv) {
    const float
            max_pos = 31,
            min_pos = 5;

    int
            nofp = atoi(argv[4]), // Número de partículas.
    nofd = 1, // Número de dimensões de fitness.
    nofi = atoi(argv[5]); // Número de iterações.

    float
            inertia = atof(argv[1]),
            cognitive = atof(argv[2]),
            social = atof(argv[3]),
            rg,
            *g_best_pos;

    particle *particles;

    run_info run = {
            .inertia = inertia,
            .cognitive = cognitive,
            .social = social,
            .iters = malloc((nofi + 1) * sizeof(iter_info)),
    };

    iter_info *initial_values;
    FILE *relat;

    srand(time(0));

    relat = fopen("relatory.json", "w");

    g_best_pos = malloc(nofd * sizeof(float));
    particles = malloc(nofp * sizeof(particle));

    // Inicializando.
    {
        // Inicializando g_best_pos.
        for (int d_index = nofd - 1; d_index >= 0; d_index--)
            g_best_pos[d_index] = 0;

        // Inicializando partículas.
        for (int p_index = nofp - 1; p_index >= 0; p_index--) {
            particle *p;

            p = &particles[p_index];

            p->pos = malloc(nofd * sizeof(float));
            p->best_pos = malloc(nofd * sizeof(float));
            p->vel = malloc(nofd * sizeof(float));

            // Inicializando pos, best_pos e vel.
            for (int d_index = nofd - 1; d_index >= 0; d_index--) {
                p->pos[d_index] = p->best_pos[d_index] =
                        ((float) rand() / (float) RAND_MAX) * (max_pos - min_pos) + min_pos;
                p->vel[d_index] =
                        ((float) rand() / (float) RAND_MAX) * (fabsf(max_pos - min_pos) - (-fabsf(max_pos - min_pos))) +
                        (-fabsf(max_pos - min_pos));
                p->cognitive = cognitive;

            }

            if (fitness(p->pos) > fitness(g_best_pos))
                for (int d_index = nofd - 1; d_index >= 0; d_index--) {
                    g_best_pos[d_index] = p->pos[d_index];

                }
        }

        initial_values = &run.iters[nofi];

        initial_values->g_best_pos = g_best_pos;
        initial_values->particles = particles;
        initial_values->rg = 0;
    }

    // Fazendo iterações.
    {
        for (int i = nofi - 1; i >= 0; i--) {
            iter_info *iter;

            iter = &run.iters[i];

            for (int p_index = nofp - 1; p_index >= 0; p_index--) {
                particle *p;

                p = &particles[p_index];

                for (int d_index = nofd - 1; d_index >= 0; d_index--) {
                    rg = ((float) rand() / (float) RAND_MAX);
                    p->r = ((float) rand() / (float) RAND_MAX);

                    p->vel[d_index] =
                            inertia * p->vel[d_index] + p->cognitive * p->r * (p->best_pos[d_index] - p->pos[d_index]) +
                            social * rg * (g_best_pos[d_index] - p->pos[d_index]);

                    if (p->vel[d_index] > fabsf(max_pos - min_pos)) p->vel[d_index] = fabsf(max_pos - min_pos);
                    if (p->vel[d_index] < -fabsf(max_pos - min_pos)) p->vel[d_index] = -fabsf(max_pos - min_pos);

                    p->pos[d_index] += p->vel[d_index];

                    if (p->pos[d_index] > max_pos) p->pos[d_index] = max_pos;
                    else if (p->pos[d_index] < min_pos) p->pos[d_index] = min_pos;

                }

                if (fitness(p->pos) > fitness(p->best_pos)) {
                    for (int d_index = nofd - 1; d_index >= 0; d_index--) {
                        p->best_pos[d_index] = p->pos[d_index];

                    }

                    if (fitness(p->best_pos) > fitness(g_best_pos))
                        for (int d_index = nofd - 1; d_index >= 0; d_index--) {
                            g_best_pos[d_index] = p->best_pos[d_index];
                        }
                }
            }

            iter->rg = rg;
            iter->particles = particles;
            iter->g_best_pos = g_best_pos;
        }
    }

    run.g_best_pos = g_best_pos;

    // Printando o relatório.
    {
        fprintf(relat, "{\"g_best_pos:\"[");
        for (int d_index = nofd - 1; d_index >= 0;  d_index--) {
            fprintf(relat, "%f", run.g_best_pos[d_index]);

            if (d_index != 0) fprintf(relat, ",");
        }
        fprintf(relat, "],");

        fprintf(relat, "\"iterations\":[");
        for (int i = nofi; i >= 0; i--) {
            iter_info *iter;

            iter = &run.iters[i];

            fprintf(relat, "{");
            fprintf(relat, "\"rg\":%f,", iter->rg);

            fprintf(relat, "\"g_best_pos\":[");
            for (int d_index = nofd - 1; d_index >= 0;  d_index--) {
                fprintf(relat, "%f", iter->g_best_pos[d_index]);

                if (d_index != 0) fprintf(relat, ",");
            }
            fprintf(relat, "],");

            fprintf(relat, "\"particles\":[");
            for (int p_index = nofp - 1; p_index >= 0; p_index--) {
                particle *p;

                p = &iter->particles[p_index];

                fprintf(relat, "{");
                fprintf(relat, "\"cognitive\":%f,", p->cognitive);
                fprintf(relat, "\"r\":%f,", p->r);

                fprintf(relat, "\"vel\":[");
                for (int d_index = nofd - 1; d_index >= 0;  d_index--) {
                    fprintf(relat, "%f", p->vel[d_index]);

                    if (d_index != 0) fprintf(relat, ",");
                }
                fprintf(relat, "],");

                fprintf(relat, "\"pos\":[");
                for (int d_index = nofd - 1; d_index >= 0;  d_index--) {
                    fprintf(relat, "%f", p->pos[d_index]);

                    if (d_index != 0) fprintf(relat, ",");
                }
                fprintf(relat, "],");

                fprintf(relat, "\"best_pos\":[");
                for (int d_index = nofd - 1; d_index >= 0;  d_index--) {
                    fprintf(relat, "%f", p->best_pos[d_index]);

                    if (d_index != 0) fprintf(relat, ",");
                }
                fprintf(relat, "]");
                fprintf(relat, "}");

                if (p_index != 0) fprintf(relat, ",");
            }
            fprintf(relat, "]");
            fprintf(relat, "}");

            if (i != 0) fprintf(relat, ",");
        }
        fprintf(relat, "]}");
    }

    // Liberando memória.
    {
        for (int p_index = nofp - 1; p_index >= 0; p_index--) {
            particle *p;

            p = &particles[p_index];

            free(p->pos);
            free(p->best_pos);
            free(p->vel);
        }

        free(run.iters);
        free(g_best_pos);
        free(particles);
    }

    printf("Program execution ended.");
    return 0;
}
