#include "distance_actions.h"


// TODO : fill body of the 2 following functions

void reset_pid_distance(uint32_t p, uint32_t i, uint32_t d)
{
    (void)p;
    (void)i;
    (void)d;
}

// move_and_measure_distance must start a defined robot move, measure differential on position, stop move when robot is doing weird, and return to starting point
bool move_and_measure_distance(double& diff_with_target, double& integrated_differential, const PID& return_pid)
{
    (void)(diff_with_target);
    (void)(integrated_differential);
    (void)(return_pid);
    return true;
}
