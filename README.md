# Description
- An implementation of concurrency lock algorithms, mainly:
    - Filter Lock:
        - Array of Flag Arrays Version
        - One Level Array Version
    - Tournament Tree with PL
    - Lamport's Bakery Lock
- Measures system throughput and turnaround time as a function of threads.

# Brief Implementation Details
- All types of lock algorithms inherit from an abstract base class called "Lock". In this way,
the function that the threads will call can use any lock function from any Lock inherited class.
- The critical section implements some shared counter.
- Turnaround Time is calculated as start of request (before lock()), and after unlock().
- The main file will run and collect results from the types of locks and measure against each other.

# Some Additional Considerations

## Atomicity and Memory Ordering
- Note that the <atomic> library is used as a safeguard to ensure that "memory re-ordering" does
not occur, and potentially void mutual exclusion. Where memory ordering is a modern optimization technique
used in most CPUs that may potentially void the order of read and write operations that will guarantee
mutual exclusion.
See https://en.wikipedia.org/wiki/Memory_ordering.
- Therefore the <atomic> library guarantees that the order that key concurrency variables may be stable.
- It is important however to note that the shared counter itself is **not** atomic, as that will void the
point of using locks.
- Otherwise concurrency is still maintained primarily through the lock algorithims, and nothing else significant actually changes.


## Running & Data Collection Concerns
- Make sure to cut out any overhead (e.g. cout debug statements, debug option in compilation, etc.) when running the program for real data.
    - Specifically, run the command
    `g++ -O3 -DNDEBUG main.cpp -o main`
    and include all the necessary other files
- Make sure to exit out of all applications to ensure that there is nothing else sharing the CPU (as much as reasonable). Includes web-browser and other applications.
- **NOTE**: Look up how to run and prep program for TACC servers.


# TODO
- [X] Implement a timer using the chrono library, and time a basic function.
- [X] Implement the timer into the simple PL algorithm.
    - [X] Time it, and form a structure to collect data over
- [X] Implement the main function, create a modular increment function that takes some Lock object and runs its lock functions.
- [X] Implement the timing of each thread. Collect system throughput and turnaround time.
- [X] Implement Bakery
    - [X] Test the bakery algorithm to ensure expected shared counter value is obtained
- [X] Implement Tournament Tree
    - [X] Test the algorithm to ensure expected shared counter value is obtained
    - [X] Found out that the issue was. Issues with how to index nodes that are not fully filled complete.
        - aka n - 1 is not the formula for amount of lock nodes
        - Assigning thread ids as pseudo children to locks is still same/similar. Add n (where n is number of nodes)
- [X] Implement main function for testing over multiple runs and data collection/structuring for just PL lock.
    - [X] Implement for the rest of the locks
- [ ] Fix the TournamentTree.xopp, as the number of nodes (locks) is incorrect and is not n - 1 purely. ()
- [ ] Use python to create plots as a function of logical thread and time.
- [X] 2 problems, 1 is deadlock: flags are both grabbed as the same index (but flag is {true, true})
~~- [ ] hash map is crashing?~~
- [X] Tournament Tree fixed!
- [ ] Now we have a different problem....
    - Lots of memory crashes and lack of freeing up pointers EVERYWHERE (check valgrind)
    - I guess the debugger was doing everything it could to free it up!
    - Create proper destructors for each Lock object and free up every instance of "new" or any
    dynamic memory allocation
    - Double check then main for any memory leaks as well.
- [ ] Create proper destructors and check for memory leaks!
    - [X] FilterLock
        - [X] FlagFilterLock
        - [X] LevelFilterLock
        - [ ] Tested in Valgrind?
    - [ ] BakeryLock
    - [ ] TournamentLock
- [ ] Double check on valgrind for leaks!


Next Time:
- keep looking at the pickFlagIndex() function
although now it seems like the hash table is the one weirding up
it is possible to write at the same time for the hash table
.erase() and [] assignment. check if that is the problem



## Questions
- How should I structure the data collected to be easily processed?
    - Vector<double> for each thread. Global vector for each thread.
    - Find average turnaround time, Find average system throughput by taking total time for each thread to complete, divided by number of CS executed.
    - Write the turnaround time into a file after joining
- Each thread should have its own "Timer" object.
- Do I want the number of CS requests to be randomized?
    - How do I have this be consistent? Seed it?




