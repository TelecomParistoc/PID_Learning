#include "optimize_pid/simulated_annealing_on_pid.h"
#include "robot_actions/distance_actions.h"
#include "robot_actions/angle_actions.h"
#include "robot_actions/fake_actions.h"

#include <iostream>
#include <motors/driver.h>

#define DEBUG true

#define REDUCTOR_P 1200
#define REDUCTOR_I 300
#define REDUCTOR_D 14000



int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    logger::debug(DEBUG);

    // Simulated_annealing_on_PID experiment(  std::bind(reset_pid_fake, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
    //                                         std::bind(move_and_measure_fake, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
    //                                         0.005, 0.3,
    //                                         PID{700, 0, 0}, 100, 3000, 0.1,
    //                                         REDUCTOR_P/2, REDUCTOR_I_D/2, REDUCTOR_I_D/2);
    // experiment.start();
    //
    // auto best_pid = experiment.best();
    // std::cout<<"Best fake PID found is : ("<<best_pid.p<<", "<<best_pid.i<<", "<<best_pid.d<<")"<<std::endl;


    Simulated_annealing_on_PID experiment(  std::bind(reset_pid_distance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                                            std::bind(move_and_measure_distance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                                            0.00001, 30.0,
                                            PID{700, 0, 0},  100, 1000, 0.1,
                                            REDUCTOR_P/2, REDUCTOR_I/2, REDUCTOR_D/2);
    experiment.start();

    auto best_pid = experiment.best();
    std::cout<<"Best distance PID found was : ("<<best_pid.p<<", "<<best_pid.i<<", "<<best_pid.d<<")"<<std::endl;
    //
    //
    // experiment = Simulated_annealing_on_PID(std::bind(reset_pid_angle, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
    //                                         std::bind(move_and_measure_angle, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
    //                                         0.1,
    //                                         PID{700, 0, 0},  100, 1000, 0.1,
    //                                         REDUCTOR_P/2, REDUCTOR_I_D/2, REDUCTOR_I_D/2);
    // experiment.start();
    //
    // best_pid = experiment.best();
    // std::cout<<"Best angle PID found was : ("<<best_pid.p<<", "<<best_pid.i<<", "<<best_pid.d<<")"<<std::endl;


    return 0;
}
