//
// Created by Tarcísio on 22/09/2023.
//

#pragma once

#ifndef PSO_APPLICATION_HEAD_H
#define PSO_APPLICATION_HEAD_H

#endif //PSO_APPLICATION_HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include "head.h"

typedef struct {
    float
            *pos,
            *best_pos,
            *vel;

    float
            r,
            cognitive;
} particle;

void free_particle(particle *p) {
    free(p->pos);
    free(p->best_pos);
    free(p->vel);
}

void calculate_particles_pos(FILE *relat, float (*fitness)(float *), int nofd, float max_pos, float min_pos, float search_area, float inertia, float cognitive, float social, int nofi, int i, int nofp, float *rg, float *g_best_pos) {
    fprintf(relat, "{\"particles\":[");
    for (int p_index = nofp - 1; p_index >= 0; p_index--) {
        particle *p = NULL;

        p = malloc(sizeof(particle));

        *rg = 0;
        p->r = 0;

        p->pos = malloc(nofd * sizeof(float));
        p->best_pos = malloc(nofd * sizeof(float));
        p->vel = malloc(nofd * sizeof(float));

        for (int d_index = nofd - 1; d_index >= 0; d_index--) {
            if (i == nofi - 1) {
                p->pos[d_index] = p->best_pos[d_index] = gen_random_in_range(max_pos, min_pos);
                p->vel[d_index] = gen_random_in_range(search_area, -search_area);
                p->cognitive = cognitive;

                if ((*fitness)(p->best_pos) > (*fitness)(g_best_pos))
                    array_cpy(p->best_pos, g_best_pos, nofd);
            }

            else {
                *rg = gen_random_in_range(1, 0);
                p->r = gen_random_in_range(1, 0);
                p->cognitive = cognitive;

                p->vel[d_index] =
                        inertia * p->vel[d_index] +
                        p->cognitive * p->r * (p->best_pos[d_index] - p->pos[d_index]) +
                        social * (*rg) * (g_best_pos[d_index] - p->pos[d_index]);

                if (p->vel[d_index] > search_area) p->vel[d_index] = search_area;
                if (p->vel[d_index] < -search_area) p->vel[d_index] = -search_area;

                p->pos[d_index] += p->vel[d_index];

                if (p->pos[d_index] > max_pos) p->pos[d_index] = max_pos;
                else if (p->pos[d_index] < min_pos) p->pos[d_index] = min_pos;
            }

            if ((*fitness)(p->pos) > (*fitness)(p->best_pos))
                array_cpy(p->pos, p->best_pos, nofd);

            if ((*fitness)(p->best_pos) > (*fitness)(g_best_pos))
                array_cpy(p->best_pos, g_best_pos, nofd);
        }

        fprintf(relat, "{\"r\":%f,\"cognitive\":%f,", p->r, p->cognitive);

        fprintf(relat, "\"vel\":[");
        print_array_to_relat(relat, p->vel, nofd);
        fprintf(relat, "],");

        fprintf(relat, "\"pos\":[");
        print_array_to_relat(relat, p->pos, nofd);
        fprintf(relat, "],");

        fprintf(relat, "\"best_pos\":[");
        print_array_to_relat(relat, p->best_pos, nofd);
        fprintf(relat, "]}");

        if (p_index != 0) fprintf(relat, ",");

        free_particle(p);
    }
    fprintf(relat, "],");
}
