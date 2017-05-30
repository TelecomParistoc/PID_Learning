#include "distance_actions.h"
#include <motors/driver.h>
#include <robotutils.h>
#include <thread>
#include <cmath>

#define DIST_TO_MOVE 400
#define DELAY_THRESHOLD 3000000.0*((double)DIST_TO_MOVE/200.0) //5000000 microseconds = 5 seconds to move
#define SPEED_DIFFERENTIAL_THRESHOLD 100.0*((double)DIST_TO_MOVE/2000.0)  // TODO: fix it


void set_linear_p(uint32_t p)
{setLinearP(p);}

void set_linear_i(uint32_t i)
{setLinearI(i);}

void set_linear_d(uint32_t d)
{setLinearD(d);}

void reset_pid_distance(uint32_t p, uint32_t i, uint32_t d)
{
    set_linear_p(p);
    set_linear_i(i);
    set_linear_d(d);
}


bool reached = false;
bool wait_wall_touched = false;
bool move_to_done = false;

void distance_reached()
{
    reached = true;
}

double get_distance()
{
    return (double)getDistance();
}

void wall_touched()
{
    printf("Wall reached\n");
    wait_wall_touched = false;
}

void move_to_callback()
{
  move_to_done = true;
}

void move_until_wall()
{
    //move_to_done = false;
    //move(-DIST_TO_MOVE+20, move_to_callback);
    //while(!move_to_done)
    //    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    wait_wall_touched = true;
    moveUntilWall(DIR_BACKWARD, wall_touched);

    while(wait_wall_touched)
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
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

    printf("Reached ? %d %f %f %f\n", reached, return_pid.p, return_pid.i, return_pid.d);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    printf("Waited\n");

    return reached;
}
