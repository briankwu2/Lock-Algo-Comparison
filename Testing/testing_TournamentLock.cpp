#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>

#include "../src/TimerWrapper.h"
#include "../src/FilterLock.h"
#include "../src/TournamentTree.h"
#include "../src/BakeryLock.h"

#define NUM_CRITICAL_SECTIONS 100000
#define NUM_PROCESSES 10

using std::thread;
using std::cout;
using std::endl;
using std::ref;

//Prototypes
void incrDebug (int &counter, const int myid, Timer &timer, Lock &lock);
void increment (int &counter, const int myid, Timer &timer, double &timeElapsed, Lock &lock);
void incrPrint (int &counter, const int myid, Lock &lock);
void displayResults(vector<double>);

int main(int argc, char const *argv[])
{
    // Use incrPrint to see the output of the threads

    vector<thread> t;
    int counter = 0;

    // Testing whether or not unlock() or lock() index counting is the issue
    // If there is a big tree, there will be multiple nodes, and even 2 processes will rest only on
    // the leaf nodes, and will have to fight to the top.
    // Results
    TournamentTree lock(NUM_PROCESSES);
    
    for (int i = 0; i < NUM_PROCESSES; i++) {
        t.emplace_back(thread(incrPrint, ref(counter), i, ref(lock))); 
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_PROCESSES; i++) {
        t[i].join();
    }

    cout << "Expected Result: " << NUM_CRITICAL_SECTIONS * NUM_PROCESSES << endl;
    cout << "Actual Result: " << counter << endl;


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
    for (int i = 0; i < NUM_CRITICAL_SECTIONS; i++)
    {
        timer.reset(); // Start Timing
        lock.lock(myid); 
        counter++;
        lock.unlock(myid); 
        timeElapsed += timer.elapsed(); // Accumulate Turnaround Time
    }

}


void incrPrint (int &counter, const int myid, Lock &lock) {
    for (int i = 0; i < NUM_CRITICAL_SECTIONS; i++) {
        lock.lock(myid);
        counter++;
        cout << "Thread " << myid << ": " << counter << endl;
        lock.unlock(myid);
    }
    cout << "Thread " << myid << " is done." << endl;

}


void displayResults (vector<double> timeE) {
    for (int i = 0; i < timeE.size(); i++) {
        cout << "Total Turnaround Time for Thread " << i << ": " << timeE[i] << " seconds" << endl;
        cout << "Average Turnaround Time for Thread " << i << ": " << timeE[i] / NUM_CRITICAL_SECTIONS << " seconds" << endl;
        cout << "System Throughput for Thread " << i << ": " << NUM_CRITICAL_SECTIONS / timeE[i] << " seconds" << endl;
        cout << endl;
    }

}
