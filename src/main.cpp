#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>

#include "TimerWrapper.h"
#include "FilterLock.h"
#include "TournamentTree.h"
#include "Bakery.h"

#define NUM_ITERATIONS 1000000

using namespace std;

//Prototypes
void incrDebug (int &counter, const int myid, Timer &timer, Lock &lock);
void increment (int &counter, const int myid, Timer &timer, double &timeElapsed, Lock &lock);
void displayResults(vector<double>);

int main(int argc, char const *argv[])
{
    int counter = 0;
    Timer timer1;
    Timer timer2;

    // thread t1(increment, ref(counter), 0, ref(timer1));
    // thread t2(increment, ref(counter), 1, ref(timer2));
    double timeE1, timeE2 = 0;
    thread t1(increment, ref(counter), 0, ref(timer1), ref(timeE1));
    thread t2(increment, ref(counter), 1, ref(timer2), ref(timeE2));
    t1.join();
    t2.join();

    cout << "Total Turnaround Time for Thread 0: " << timeE1 << " seconds" << endl;
    cout << "Total Turnaround Time for Thread 1: " << timeE2 << " seconds" << endl;
    cout << endl;
    cout << "Average Turnaround Time for Thread 0: " << timeE1 / NUM_ITERATIONS << endl;
    cout << "Average Turnaround Time for Thread 1: " << timeE2 / NUM_ITERATIONS << endl;
    cout << endl;

    cout << "System Throughput for Thread 0: " << NUM_ITERATIONS / timeE1 << endl;
    cout << "System Throughput for Thread 1: " << NUM_ITERATIONS / timeE2 << endl;

    cout << "All Done!" << endl;
    return 0;
}

/**
 * @brief More of a testing function that will print how much time has elapsed
 * to check whether or not any threads are getting starved.
 * Includes a "cout" within the CS, so is not true to the actual turnaround time.
 * 
 * @param counter 
 * @param myid 
 * @param timer 
 * @param lock 
 */
void incrDebug (int &counter, const int myid, Timer &timer, Lock &lock) {
    double timeElapsed;
    int predCount;
    for (int i = 0; i < 100; i++)
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

// Note: Consider bundling all the reference variables into some struct, and just pass the struct instead
void increment (int &counter, const int myid, Timer &timer, double &timeElapsed, Lock &lock) {
    int predCount;
    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        timer.reset(); // Start Timing
        lock.lock(myid); 
        counter++;
        lock.unlock(myid); 
        timeElapsed += timer.elapsed(); // Accumulate Turnaround Time
    }

}