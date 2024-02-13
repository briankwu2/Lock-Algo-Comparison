#ifndef DATA_FUNCTIONS_H
#define DATA_FUNCTIONS_H
#include "TimerWrapper.h"
#include "Lock.h"
#include <vector>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;


/**
 * @brief Standalone header file for data functions. 
 * Implementation is included in the header file.
 */


// Parameters to be changed for testing
#define NUM_ITERATIONS 1000000
#define NUM_PROCESSES 2 

// Prototypes to see what function are implemented
// NOTE: Always change the prototype if the function is changed 
void increment (ThreadParameters &tp);
void incrDebug (ThreadParameters &tp);

// Structs --------------------------------------

/**
 * @brief Struct that will be used to pass parameters to a thread.
 * 
 */
struct ThreadParameters {
public:
    const int myid;
    double &timeElapsed;
    int &counter;
    Timer &tpTimer;
    Timer &sysTimer;
    Lock *lock;

    ThreadParameters (const int id, double &te, int &c, Timer &tp, Timer &sys, Lock *l)
        : myid{id}
        , timeElapsed{te}
        , counter{c}
        , tpTimer{tp}
        , sysTimer{sys}
        , lock{l} 
        {}

};


// Implementation --------------------------------------
/**
 * @brief More of a testing function that will print how much time has elapsed
 * to check whether or not any threads are getting starved.
 * Includes a "cout" within the CS, so is not true to the actual turnaround time.
 * 
 * @param tp // ThreadParameters struct that will be used to pass parameters to the function.
 */
void incrDebug (ThreadParameters &tp) {
    double timeElapsed = tp.timeElapsed;
    int predCount;
    int &counter = tp.counter;
    const int myid = tp.myid;
    Timer &timer = tp.tpTimer;
    Lock &lock = *tp.lock;

    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        timer.reset();
        lock.lock(myid); 
        counter++;
        predCount = counter;;
        // cout << "Thread " << myid << ": " << counter << endl; 
        timeElapsed = timer.elapsed();
        cout << "Thread " << myid << ": " << predCount << "| Time Taken: " << timeElapsed << endl;
        lock.unlock(myid);
        // Bottom Lines will measure without overhead from cout, but will have weird formatting
        // timeElapsed = timer.elapsed();
        // cout << "Thread " << myid << ": " << predCount << "| Time Taken: " << timeElapsed << endl;
    }
}


/**
 * @brief Actual critical section that will be used to test the performance of the locks.
 * Is passed to a thread and will be called by the thread.
 * Measures the time taken to increment a shared variable.
 * As well as the overall time taken to run the function from start to finish.
 * 
 * @param tp 
 */
void increment (ThreadParameters &tp) {

    double &timeElapsed = tp.timeElapsed;
    int &counter = tp.counter;
    const int myid = tp.myid;
    Timer &tpTimer = tp.tpTimer;
    Timer &sysTimer = tp.sysTimer;
    Lock &lock = *tp.lock;



    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        timer.reset(); // Start Timing
        lock.lock(myid); 
        counter++;
        lock.unlock(myid); 
        timeElapsed += timer.elapsed(); // Accumulate Turnaround Time
    }

}

#endif // DATA_FUNCTIONS_H
