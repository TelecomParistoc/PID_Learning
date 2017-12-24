#include "distance_actions.h"

#include <thread>


// TODO : fill body of the 2 following functions

uint32_t _p, _i, _d;

void reset_pid_distance(uint32_t p, uint32_t i, uint32_t d)
{
    _p = p;
    _i = i;
    _d = d;
}

// move_and_measure_distance must start a defined robot move, measure differential on position, stop move when robot is doing weird, and return to starting point
bool move_and_measure_distance(double& diff_with_target, double& integrated_differential)
{
    diff_with_target = (_p > 3000)?0:(float)_p/3000.0;
    integrated_differential = sqrt(fabs(_i-5000))/(float)10+fabs(_d-1000)*fabs(_d-1000)/(float)1000000;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return true;
}
