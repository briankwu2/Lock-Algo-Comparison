#ifndef DATA_FUNCTIONS_H
#define DATA_FUNCTIONS_H

#include <vector>
#include <iostream>
#include <fstream>

#include "TimerWrapper.h"
#include "FilterLock.h"
#include "TournamentTree.h"
#include "BakeryLock.h"

using std::vector;
using std::cout;
using std::endl;
using std::ofstream;

/**
 * @brief Standalone header file for data functions. 
 * Implementation is included in the header file.
 */


// Parameters to be changed for testing
#define NUM_CRITICAL_SECTIONS 1000
#define NUM_THREADS 4 
#define NUM_ITERATIONS 2 // Number of times to run the test for each number of threads

// Structs --------------------------------------

/**
 * @brief Struct that will be used to pass parameters to a thread.
 * 
 */
struct ThreadParameters {
public:
    const int myid;
    long double &timeElapsed;
    long double &sysTimeElapsed;
    int &counter;
    Timer &tpTimer;
    Timer &sysTimer;
    Lock *lock;

    ThreadParameters (const int id, long double &te, long double &ste, int &c, Timer &tp, Timer &sys, Lock *l)
        : myid{id}
        , timeElapsed{te}
        , sysTimeElapsed{ste}
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
    long double timeElapsed = tp.timeElapsed;
    int predCount;
    int &counter = tp.counter;
    const int myid = tp.myid;
    Timer &timer = tp.tpTimer;
    Lock &lock = *tp.lock;

    for (int i = 0; i < NUM_CRITICAL_SECTIONS; i++)
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

    long double &timeElapsed = tp.timeElapsed;
    long double &sysTimeElapsed = tp.sysTimeElapsed;
    int &counter = tp.counter;
    const int myid = tp.myid;
    Timer &tpTimer = tp.tpTimer;
    Timer &sysTimer = tp.sysTimer;
    Lock &lock = *tp.lock;

    sysTimer.reset(); // Start Timing the System
    for (int i = 0; i < NUM_CRITICAL_SECTIONS; i++)
    {
        tpTimer.reset(); // Start Timing
        lock.lock(myid); 
        counter++;
        lock.unlock(myid); 
        timeElapsed += tpTimer.elapsed(); // Accumulate Turnaround Time
    }

    sysTimeElapsed = sysTimer.elapsed(); // Accumulate System Time


}

/**
 * @brief Helper function used to return a pointer to a Lock object that holds a
 * derived Lock class. To be used in a modular critical section function.
 * 
 * @param lockType 
 * @param numThreads 
 * @return Lock* 
 */
Lock *selectLock(int lockType, int numThreads) {
    Lock *lock;
    switch (lockType) {
        case 0:
            lock = new FlagFilterLock(numThreads);
            break;
        case 1:
            lock = new LevelFilterLock(numThreads);
            break;
        case 2:
            lock = new TournamentTree(numThreads);
            break;
        case 3:
            lock = new BakeryLock(numThreads);
            break;
        default:
            cout << "Invalid lock type" << endl;
            lock = nullptr;
    }
    return lock;
}

/**
 * @brief Prepares a file to write to and returns a output file stream.
 * 
 * @param lockType 
 * @param thread_num 
 * @param folderName 
 * @return ofstream 
 */
ofstream prepareFile (int lockType, int thread_num, std::string folderName = "./data/") {
    ofstream file;

    switch (lockType) {
        case 0:
            file.open(folderName + "FlagFilterLock" + std::to_string(thread_num) + ".csv");
            break;
        case 1:
            file.open(folderName + "LevelFilterLock" + std::to_string(thread_num) + ".csv");
            break;
        case 2:
            file.open(folderName + "TournamentTree" + std::to_string(thread_num) + ".csv");
            break;
        case 3:
            file.open(folderName + "BakeryLock" + std::to_string(thread_num) + ".csv");
            break;
        default:
            cout << "Invalid lock type" << endl;
            return file;
    }
    if (!file.is_open()) {
        cout << "File failed to open. Check if there is a proper data folder." << endl;
        cout << "Default is ./data/ in the same directory as the executable." << endl;
    }

    return std::move(file);

}

/**
 * @brief Prepares the file to store the final and averaged results. Returns a output file stream.
 * 
 * @param lockType 
 * @param folderName 
 * @return ofstream 
 */
ofstream prepareAvgFile (int lockType, std::string folderName = "./data/") {
    ofstream file;

    switch (lockType) {
        case 0:
            file.open(folderName + "AvgFlagFilterLock.csv");
            break;
        case 1:
            file.open(folderName + "AvgLevelFilterLock.csv");
            break;
        case 2:
            file.open(folderName + "AvgTournamentTree.csv");
            break;
        case 3:
            file.open(folderName + "AvgBakeryLock.csv");
            break;
        default:
            cout << "Invalid lock type" << endl;
            return file;
    }
    if (!file.is_open()) {
        cout << "File failed to open. Check if there is a proper data folder." << endl;
        cout << "Default is ./data/ in the same directory as the executable." << endl;
    }

    return std::move(file);

}
#endif // DATA_FUNCTIONS_H
