//
// Created by Tarcísio on 22/09/2023.
//

#pragma once

#ifndef PSO_APPLICATION_HEAD_H
#define PSO_APPLICATION_HEAD_H

#endif //PSO_APPLICATION_HEAD_H

void array_cpy(float *arr1, float *arr2, int arr1_len) {
    for (int i = arr1_len - 1; i >= 0; i--) {
        arr2[i] = arr1[i];
    }
}

void set_array(float *arr, int arr_len) {
    for (int i = arr_len - 1; i >= 0; i--) {
        arr[i] = 0;
    }
}

void print_array_to_relat(FILE *relat, float *arr, int arr_len) {
    for (int i = arr_len - 1; i >= 0; i--) {
        fprintf(relat, "%f", arr[i]);

        if (i != 0) fprintf(relat, ",");
    }
}

float gen_random_in_range(float max, float min) {
    return ((float) rand() / (float) RAND_MAX) * (max - min) + min;
}
