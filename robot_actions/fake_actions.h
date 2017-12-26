#ifndef FAKE_ACTIONS_H
#define FAKE_ACTIONS_H


#include "../optimize_pid/pid.h"

#include <stdint.h>


void reset_pid_fake(uint32_t p, uint32_t i, uint32_t d);
bool move_and_measure_fake(double& diff_with_target, double& integrated_differential, const PID& return_pid);


#endif
