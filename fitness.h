//
// Created by Tarcísio on 22/09/2023.
//
#pragma once

#ifndef PSO_APPLICATION_FITNESS_H
#define PSO_APPLICATION_FITNESS_H

#endif //PSO_APPLICATION_FITNESS_H

#include <math.h>

float fitness(float *input) {
    float  x = input[0];
    return (12 * powf(x, 5) - 975 * powf(x, 4) + 28000 * powf(x, 3) - 345000 * powf(x, 2) + 1800000 * x) / 1000000;
}
