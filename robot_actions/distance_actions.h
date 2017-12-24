#ifndef DISTANCE_ACTIONS_H
#define DISTANCE_ACTIONS_H


#include <stdint.h>


void reset_pid_distance(uint32_t p, uint32_t i, uint32_t d);
bool move_and_measure_distance(double& diff_with_target, double& integrated_differential);


#endif
