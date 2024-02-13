#ifndef TIMER_WRAPPER_H
#define TIMER_WRAPPER_H
#include <chrono>

/*
Class Wrapper taken and slightly modified from https://www.learncpp.com/cpp-tutorial/timing-your-code/.
*/


// Define the type of second to be used
enum SecondType {
    NANOSECONDS = 0,
    MICROSECONDS = 1,
    MILLISECONDS = 2,
    SECONDS = 3
};

/**
 * @brief Class Wrapper around std::chrono.
 * Allows a simple snapshot of time in nanoseconds 
 */
class Timer {
private:
    const int secondType;

    // Type Aliases
    using Clock = std::chrono::steady_clock;
    std::chrono::time_point<Clock> beginning {Clock::now()};

public:

    // Defaults to seconds
    Timer(int i = SECONDS) 
        : secondType{i}
    {}

    /**
     * @brief Resets when the timer should begin recording time.
     * 
     */
    void reset() {
        beginning = Clock::now();
    }

    /**
     * @brief Returns how much time has elapsed since a reset() or the creation of the Timer object.
     * Depending on the secondType, the time is returned in nanoseconds, microseconds, milliseconds, or seconds. 
     * @return double long 
     */
    long double elapsed() const {
        switch (secondType) {
            case NANOSECONDS:
                return std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - beginning).count();
                break;
            case MICROSECONDS:
                return std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - beginning).count();
                break;
            case MILLISECONDS:
                return std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - beginning).count();
                break;
            case SECONDS:
                return std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - beginning).count();
                break;
            default:  // Default is nanoseconds
                return std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - beginning).count();
        }
    }

};

#endif // TIMER_WRAPPER_H