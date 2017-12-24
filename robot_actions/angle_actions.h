#ifndef ANGLE_ACTIONS_H
#define ANGLE_ACTIONS_H


void reset_pid_angle(uint8_t p, uint8_t i, uint8_t d);
bool move_and_measure_angle(double& diff_with_target, double& integrated_differential);


#endif
