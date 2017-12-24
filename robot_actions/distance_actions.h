#ifndef DISTANCE_ACTIONS_H
#define DISTANCE_ACTIONS_H


void reset_pid_distance(uint8_t p, uint8_t i, uint8_t d);
bool move_and_measure_distance(double& diff_with_target, double& integrated_differential);


#endif
