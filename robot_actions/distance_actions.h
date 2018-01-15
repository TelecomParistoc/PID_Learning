#ifndef DISTANCE_ACTIONS_H
#define DISTANCE_ACTIONS_H


#include "../optimize_pid/pid.h"

#include "logging/include/loggers/common_loggers.h"

#include <stdint.h>

#define X_ORIGIN 0
#define Y_ORIGIN 0
#define ORIGIN_ORIENTATION 0

void reset_pid_distance(uint32_t p, uint32_t i, uint32_t d);
bool move_and_measure_distance(double& total_delay, double& integrated_differential, const PID& return_pid);


#endif
