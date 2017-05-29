#ifndef DISTANCE_ACTIONS_H
#define DISTANCE_ACTIONS_H


#include "../optimize_pid/pid.h"

#include <stdint.h>

#define X_ORIGIN 1000
#define Y_ORIGIN 1000


void reset_pid_distance(uint32_t p, uint32_t i, uint32_t d);
bool move_and_measure_distance(double& delay, double& integrated_differential, const PID& return_pid);


#endif
