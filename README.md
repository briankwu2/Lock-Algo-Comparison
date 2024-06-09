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



