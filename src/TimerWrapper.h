#ifndef TIMER_WRAPPER_H
#define TIMER_WRAPPER_H
#include <chrono>

/*
Class Wrapper taken and slightly modified from https://www.learncpp.com/cpp-tutorial/timing-your-code/.
*/

/**
 * @brief Class Wrapper around std::chrono.
 * Allows a simple snapshot of time in seconds 
 */
class Timer {
private:
    // Type Aliases
    using Clock = std::chrono::steady_clock;
    using Second = std::chrono::duration<double, std::ratio<1> >;

    std::chrono::time_point<Clock> beginning {Clock::now()}; // Keeps time stamp of when the Timer class is instantiated

public:
    /**
     * @brief Resets when the timer should begin recording time.
     * 
     */
    void reset() {
        beginning = Clock::now();
    }

    /**
     * @brief Returns how much time has elapsed since a reset() or the creation of the Timer object.
     * 
     * @return double - seconds that have passed since reset() or initialization
     */
    double elapsed() const {
        return std::chrono::duration_cast<Second>(Clock::now() - beginning).count();
    }

};

#endif // TIMER_WRAPPER_H