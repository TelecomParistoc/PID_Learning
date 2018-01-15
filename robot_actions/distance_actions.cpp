#include "distance_actions.h"
#include <motors/driver.h>
#include <robotutils.h>
#include <thread>
#include <cmath>


#define DIST_TO_MOVE 2000
#define DELAY_THRESHOLD 5000000.0*((double)DIST_TO_MOVE/2000.0) //5000000 microseconds = 5 seconds to move
#define SPEED_DIFFERENTIAL_THRESHOLD 10.0*((double)DIST_TO_MOVE/2000.0)  // TODO: fix it

//binding with libmotors
void set_linear_p(uint32_t p){
  setLinearP((int) p);
}
void set_linear_i(uint32_t i){
  setLinearI((int) i);
}
void set_linear_d(uint32_t d){
  setLinearD((int) d);
}


void reset_pid_distance(uint32_t p, uint32_t i, uint32_t d)
{
    set_linear_p(p);
    set_linear_i(i);
    set_linear_d(d);
}


bool reached = false;

void distance_reached()
{
    reached = true;
}

double get_distance()
{
    //TODO: bind with libmotors to get current distance
    return 0;
}

void move_until_wall()
{
    //TODO: implement recalibration
}

// move_and_measure_distance must start a defined robot move, measure differential on position, stop move when robot is doing weird, and return to starting point
bool move_and_measure_distance(double& total_delay, double& integrated_differential, const PID& return_pid)
{
    total_delay = 0;
    integrated_differential = 0;

    reached = false;
    move(DIST_TO_MOVE, distance_reached);

    bool first_speed = true, first_accel = true;

    double delay = 0, prev_delay = 0;
    double cur_distance, prev_distance, cur_speed = 0, prev_speed, cur_accel, prev_accel = 0;
    std::chrono::time_point<std::chrono::system_clock> pre_previous = std::chrono::system_clock::now(), previous = pre_previous, current = previous;
    while(!reached && total_delay<DELAY_THRESHOLD && integrated_differential<SPEED_DIFFERENTIAL_THRESHOLD)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        prev_distance = cur_distance;
        cur_distance = get_distance();
        pre_previous = previous;
        previous = current;
        current = std::chrono::system_clock::now();
        delay = std::chrono::duration_cast<std::chrono::microseconds>(current - previous).count();
        prev_delay = std::chrono::duration_cast<std::chrono::microseconds>(previous - pre_previous).count();
        total_delay += delay;

        if(first_speed)
        {
            first_speed = false;
            prev_speed = (cur_distance-prev_distance)/delay;
        }
        else
        {
            prev_speed = cur_speed;
            cur_speed = (cur_distance-prev_distance)/delay;
            cur_accel = fabs((cur_speed-prev_speed)/(prev_delay+delay));
            if(first_accel)
            {
                first_accel = false;
                integrated_differential += cur_accel*prev_delay;
                prev_accel = cur_accel;
            }
            else
            {
                integrated_differential += (cur_accel+prev_accel)/2.0*prev_delay;
                prev_accel = cur_accel;
            }
        }
    }

    set_linear_p(return_pid.p);
    set_linear_i(return_pid.i);
    set_linear_d(return_pid.d);

    move_until_wall();

    return reached;
}
