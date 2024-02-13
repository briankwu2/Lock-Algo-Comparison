// #include <iostream>
// #include <vector>
// #include <atomic>
// #include <thread>
// #include <chrono>


// // User Defined Headers
// #include "TimerWrapper.h"
// #include "FilterLock.h"
// #include "TournamentTree.h"
// #include "BakeryLock.h"
// #include "dataFunctions.h"


// // Namespaces
// using std::thread;
// using std::cout;
// using std::endl;
// using std::ref;



/**
 * @brief Main function that tests and measures the performance of different locks.
 * Collects and outputs into separate files, the turnaround time and system throughput for each lock type for
 * some number of threads. 
 * e.g. Collects data for 2, 4, 8, 16, 32, 64, 128 threads.
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
// int main(int argc, char const *argv[])
// {
//     // Different Types of Locks
//     FlagFilterLock flagFilterLock(NUM_PROCESSES);
//     LevelFilterLock levelFilterLock(NUM_PROCESSES);
//     BakeryLock bakeryLock(NUM_PROCESSES);
//     TournamentTree tourneyLock(NUM_PROCESSES);


//     // Shared Variables and Separate Timers
//     int counter = 0; // Shared Variable




//     return 0;
// }

