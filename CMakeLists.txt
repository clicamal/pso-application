cmake_minimum_required(VERSION 3.26)
project(pso_application C)

set(CMAKE_C_STANDARD 99)

add_executable(pso_application main.c
        calculate_particles_pos.h
        head.h
        do_iterations.h
        fitness.h)
