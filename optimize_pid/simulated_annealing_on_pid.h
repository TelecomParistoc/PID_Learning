#ifndef OPTIMIZE_PID_H
#define OPTIMIZE_PID_H


#include "simulated_annealing/simulated_annealing.h"
#include "logging/include/loggers/common_loggers.h"

#include <chrono>


#define MAX 0xffffffff


class PID
{
    public:
        double p, i, d;
};


template <double k, double k_prim>  // parameters of weight function : weight = |diff_with_goal| + k*delay + kprim*integrated_differential where :
                                    // - diff_with_goal is difference between estimated and targeted robot position
                                    // - delay is time robot takes to perform chosen command
                                    // - integrated_differential is the integral value of absolute measured position derivative
class Simulated_annealing_on_PID
{
    public:
        /**
            Constructor of particular robot experiment is done with following arguments :
                - reset_pid :           function that resets PID on robot.
                                                The function must takes 3 integer arguments :
                                                    - P value
                                                    - I value
                                                    - D value
                - move_and_measure :    function that starts robot movement, measures parameters like current position and derivative, and orders robot going back to the origin point when main measured move is finished.
                                                The function must return true if move succeeded, false otherwise.
                                                It must take 2 arguments :
                                                    - reference to variable that will contain difference between estimated and targeted robot position
                                                    - reference to variable that will contain the integral value of absolute measured position derivative
                - initial_pid :         parameter that corresponds to initial (non optimized but ok) PID which will be set
                - initial_temperature : parameter that influences exploration/exploitation ratio : high initial temperature means more exploration/less exploitation
                - n_iterations :        parameter that controls number of PID that will be tested by the robot
                - target :              parameter that corresponds to final temperature
                - P_interval :          parameter that bounds possible gap between 2 successive P values
                - I_interval :          parameter that bounds possible gap between 2 successive I values
                - D_interval :          parameter that bounds possible gap between 2 successive D values
        **/
        Simulated_annealing_on_PID(const std::function<void(uint32_t, uint32_t, uint32_t)>& reset_pid, const std::function<bool(double&, double&)>& move_and_measure, const PID& initial_pid, double initial_temperature = 100, size_t n_iterations = 1000, double target = 0.1, double P_interval = 1, double I_interval = 1, double D_interval = 1);

        double weight(const PID& pid);      // weight method performs an iteration of simutaed annealing algorithm :
                                            //    - robot pid is reset
                                            //    - move order is given to robot and result is measured
                                            //    - if move was ok, weight of the move is returned

        double temperature(size_t i, double prev_temperature);  // classic simulated annealing temperature (geometric non increasing progression X^i)
        PID neighbour(const PID& pid);                          // neighbour is determined by a random uniform value in interval

        void start();
        const PID& best() const;

    private:
        std::function<void(uint32_t, uint32_t, uint32_t)> _reset_pid_function;
        std::function<bool(double&, double&)> _move_and_measure_function,

        PID _initial_pid;
        PID _best_pid;
        double _best_weight;

        double _initial_temperature;
        size_t _n_iterations;
        double _coeff;

        std::random_device _random_device;
        std::default_random_engine _generator;
        std::uniform_real_distribution<double> _distrib_p, _distrib_i, _distrib_d;
};



template <double k, double k_prim>
Simulated_annealing_on_PID<k, k_prim>::Simulated_annealing_on_PID(const std::function<void(uint32_t, uint32_t, uint32_t)>& reset_pid, const std::function<bool(double&, double&)>& move_and_measure, const PID& initial_pid, double initial_temperature, size_t n_iterations, double target, double P_interval, double I_interval, double D_interval) :
    _reset_pid_function(reset_pid),
    _move_and_measure_function(move_and_measure),
    _initial_pid(initial_pid),
    _initial_temperature(initial_temperature),
    _n_iterations(n_iterations),
    _generator(_random_device),
    _distrib_p(-P_interval/2, P_interval/2),
    _distrib_i(-I_interval/2, I_interval/2),
    _distrib_d(-D_interval/2, D_interval/2)
{
    _coeff = exp(log(target)/(double)(_n_iterations))>0.99999999?0.99999999:exp(log(target)/(double)(_n_iterations)); // coeff^n_iterations = target (exp(n_iterations*ln(coeff)) = target)
}

template <double k, double k_prim>
double Simulated_annealing_on_PID<k, k_prim>::weight(const PID& pid)
{
    if(pid.p < 0)
    {
        logger::write_endline("Not weighted because pid.p is negative");
        return MAX;
    }

    logger::write_endline("Testing PID ", pid.p, pid.i, pid.d);
    reset_pid((uint32_t)pid.p, (uint32_t)pid.i, (uint32_t)pid.d);

    std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();

    double diff_goal;
    double integrated_differential;
    if(!move_and_measure(diff_goal, integrated_differential)) // move failed or too much difference with target
    {
        logger::write_endline("Not weighted because move failed");
        return MAX;
    }

    std::chrono::time_point<std::chrono::system_clock> current = std::chrono::system_clock::now();
    double delay = std::chrono::duration_cast<std::chrono::milliseconds>(current - begin);

    double res = fabs(diff_goal)+k*delay+kprim*integrated_differential;

    logger::write_endline("Measured parameters :");
    logger::write_endline("diff with goal :", diff_goal);
    logger::write_endline("delay :", delay);
    logger::write_endline("differential : ", differential);
    logger::write_endline("Final weight : ", res);

    if(res < _best_weight)
    {
        _best_pid = pid;
        _best_weight = res;
        logger::write_endline("Better PID found : (", pid.p, ", ", pid.i, ", ", pid.d, ")");
    }

    return res;
}

template <double k, double k_prim>
double Simulated_annealing_on_PID<k, k_prim>::temperature(size_t i, double prev_temperature)
{
    (void)i;
    if(prev_temperature < 0)
        return _initial_temperature;
    else
        return _coeff*prev_temperature;
}

template <double k, double k_prim>
PID Simulated_annealing_on_PID<k, k_prim>::neighbour(const PID& pid)
{
    return PID{pid.p+_distrib_p(_generator), pid.i+_distrib_i(_generator), pid.d+_distrib_d(_generator)};
}

template <double k, double k_prim>
void Simulated_annealing_on_PID<k, k_prim>::start()
{
    logger::write_endline("Starting a new experiment with multiplicative coeff for temperature : ", coeff, " => giving coeff^n_iterations = ", pow(coeff, n_iterations));

    _best_pid = simulated_annealing<PID, double, double>(_n_iterations, _initial_pid,
                                        std::bind(Simulated_annealing_on_PID<k, k_prim>::weight, this, std::placeholders::_1, _reset_pid_function, _move_and_measure_function),
                                        std::bind(Simulated_annealing_on_PID<k, k_prim>::temperature, this, std::placeholders::_1, std::placeholders::_2),
                                        std::bind(Simulated_annealing_on_PID<k, k_prim>::neighbour, this, std::placeholders::_1));
}

template <double k, double k_prim>
const PID& Simulated_annealing_on_PID<k, k_prim>::best() const
{
    return _best_pid;
}


#endif
