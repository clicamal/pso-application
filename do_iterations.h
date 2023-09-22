//
// Created by Tarcísio on 22/09/2023.
//
#pragma once

#ifndef PSO_APPLICATION_DO_ITERATIONS_H
#define PSO_APPLICATION_DO_ITERATIONS_H

#endif //PSO_APPLICATION_DO_ITERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "head.h"
#include "calculate_particles_pos.h"

void do_iterations(float (*fitness)(float *), int nofd, float min_pos, float max_pos, float inertia, float social, float cognitive, int nofp, int nofi) {
    const float search_area = fabsf(max_pos - min_pos);

    float
            rg,
            *g_best_pos = NULL;

    FILE *relat = NULL;

    relat = fopen("relatory.json", "w");

    g_best_pos = malloc(nofd * sizeof(float));

    // Inicializando g_best_pos.
    set_array(g_best_pos, nofd);

    fprintf(relat, "{\"inertia\":%f,", inertia);
    fprintf(relat, "\"cognitive\":%f,", cognitive);
    fprintf(relat, "\"social\":%f,", social);
    fprintf(relat, "\"iterations\":[");
    for (int i = nofi - 1; i >= 0; i--) {
        calculate_particles_pos(relat, fitness, nofd, max_pos, min_pos, search_area, inertia, cognitive, social,  nofi, i, nofp, &rg, g_best_pos);

        fprintf(relat, "\"g_best_pos\":[");
        print_array_to_relat(relat, g_best_pos, nofd);
        fprintf(relat, "]}");

        if (i != 0) fprintf(relat, ",");
    }
    fprintf(relat, "],");

    fprintf(relat, "\"g_best_pos\":[");
    printf("gbest: ");
    for (int d_index = nofd - 1; d_index >= 0; d_index--) {
        float d_value = g_best_pos[d_index];

        fprintf(relat, "%f", d_value);
        printf("%f ", d_value);

        if (d_index != 0) fprintf(relat, ",");
    }
    printf("\n");
    fprintf(relat, "]}");

    free(g_best_pos);
    fclose(relat);
}
