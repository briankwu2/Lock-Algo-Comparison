#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include <fstream>
#include <iomanip>

// User Defined Headers
#include "TimerWrapper.h"
#include "FilterLock.h"
#include "TournamentTree.h"
#include "BakeryLock.h"
#include "dataFunctions.h"


// Namespaces
using std::thread;
using std::cout;
using std::endl;
using std::ref;
using std::ofstream;

// NOTE:
// Parameters to be changed for testing can be found in the dataFunctions.h file

/**
 * @brief Main function that tests and measures the performance of different locks.
 * Collects and outputs into separate files, the turnaround time and system throughput for each lock type for
 * some number of threads. 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char const *argv[])
{
    // Shared Variables and Separate Timers
    int counter = 0; // Shared Variable

    // Loop through each lock type
    for (int lockType = 0; lockType < 4; lockType++) {


        ofstream finalAvgFile = prepareAvgFile(lockType);
        finalAvgFile << std::fixed << std::setprecision(3); // Sets the precision of numbers written into the file
        finalAvgFile << "Number of Threads,Average Turnaround Time (ns),Average System Throughput" << endl;

        // Loop through each number of threads
        for (int i = 1; i <= NUM_THREADS; i++) {

            // Prepare the file for writing for this lock type.
            ofstream file = prepareFile(lockType, i);
            file << std::fixed << std::setprecision(3); // Sets the precision of numbers written into the file

            long double allThreadsAverageTT = 0;
            long double allThreadsAverageST = 0;

            finalAvgFile << i << ","; // Indicates the number of threads for the average data file 

            file << "Iteration,Thread Number,TurnaroundTime(ns),SystemThroughput(CS/s)" << endl;

            // Test the number of threads for some amount of iterations
            for (int iteration = 0; iteration < NUM_ITERATIONS; iteration++) {
                // Select lock type;
                Lock *lock = selectLock(lockType, i); 

                // Instantiate a vector of necessary thread variables for the number of threads
                vector<thread> threads;
                vector<ThreadParameters> tpVec;
                vector<long double> timeElapsedVec;
                vector<long double> sysTimeElapsedVec;
                vector<Timer> tpTimerVec;
                vector<Timer> sysTimerVec;

                // Pre-allocate space for the vectors (for efficiency) 
                // Except for threads as it needs in-place construction
                tpVec.reserve(i);
                timeElapsedVec.reserve(i);
                sysTimeElapsedVec.reserve(i);
                tpTimerVec.reserve(i);
                sysTimerVec.reserve(i);
                
                // Instantiate each thread parameter and timer
                for (int j = 0; j < i; j++) {
                    timeElapsedVec.push_back(0);
                    sysTimeElapsedVec.push_back(0);
                    tpTimerVec.push_back(Timer(NANOSECONDS));
                    sysTimerVec.push_back(Timer(NANOSECONDS));
                    tpVec.push_back(ThreadParameters(
                        j, 
                        ref(timeElapsedVec[j]),
                        ref(sysTimeElapsedVec[j]),
                        ref(counter),
                        ref(tpTimerVec[j]),
                        ref(sysTimerVec[j]),
                        ref(lock))
                    );
                }

                // Begin the threads
                for (int j = 0; j < i; j++) {
                    threads.emplace_back(thread(increment, ref(tpVec[j])));
                }

                // Wait for all threads to finish
                for (int j = 0; j < i; j++) {
                    threads[j].join();
                }


                // Collect data from Thread Parameters and write to the file
                for (int j = 0; j < i; j++) {
                    // Calculations of average turnaround time and average system throughput
                    long double averageTurnaroundTime = timeElapsedVec[j] / NUM_CRITICAL_SECTIONS; 
                    long double averageSystemThroughput = NUM_CRITICAL_SECTIONS / (sysTimeElapsedVec[j] * .000000001);

                    // Write to file
                    file << iteration << "," << j << "," << averageTurnaroundTime << "," << averageSystemThroughput << endl;

                    allThreadsAverageTT += averageTurnaroundTime;
                    allThreadsAverageST += averageSystemThroughput;
                }

                // De-allocate lock memory & close file
                delete lock;

            } // END OF ITERATION #

            file << endl; // Separates Number of Threads for raw file

            finalAvgFile << allThreadsAverageTT / (i * NUM_ITERATIONS) << ",";
            finalAvgFile << allThreadsAverageST / (i * NUM_ITERATIONS) << endl;

            file.close();

        } // END OF THREADS LOOP

        // Close the file
        finalAvgFile.close();

    } // END OF LOCK TYPE

    cout << "Program has finished running." << endl;
    return 0;
}




