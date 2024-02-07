# Description
- An implementation of concurrency lock algorithms, mainly:
    - Filter Lock:
        - Array of Flag Arrays Version
        - One Level Array Version
    - Tournament Tree with GPL
    - Lamport's Bakery Lock
- Measures system throughput and turnaround time as a function of threads.

# Brief Implementation Details
- All types of lock algorithms inherit from an abstract base class called "Lock"
- The main file will run and collect results from the types of locks and measure against each other
- The critical section implements some shared counter.


# Some Additional Considerations

## Atomicity and Memory Ordering
- Note that the <atomic> library is used as a safeguard to ensure that "memory re-ordering" does
not occur, and potentially void mutual exclusion. Where memory ordering is a modern optimization technique
used in most CPUs that may potentially void the order of read and write operations that will guarantee
mutual exclusion.
See https://en.wikipedia.org/wiki/Memory_ordering.
- Therefore the <atomic> library guarantees that the order that key concurrency variables may be stable.
- Otherwise concurrency is still maintained primarily through the lock algorithims, and nothing else significant actually changes.

## Running & Data Collection Concerns
- Make sure to cut out any overhead (e.g. cout debug statements, debug option in compilation, etc.) when running the program for real data.
- Make sure to exit out of all applications to ensure that there is nothing else sharing the CPU (as much as reasonable). Includes web-browser and other applications.

# TODO
- [X] Implement a timer using the chrono library, and time a basic function.
- [X] Implement the timer into the simple PL algorithm.
    - [ ] Time it, and form a structure to collect data over
        1. Multiple Runs of the program
        2. Into a file without disrupting the performance
- [ ] Implement the main function, create a modular increment function that takes some Lock object and runs its lock functions.
- [X] Implement the timing of each thread. Collect system throughput and turnaround time.
    - [ ] Use python to create plots as a function of logical thread and time.

## Questions
- How should I structure the data collected to be easily processed?
    - Vector<double> for each thread. Global vector for each thread.
    - Find average turnaround time, Find average system throughput by taking total time for each thread to complete, divided by number of CS executed.
    - Write the turnaround time into a file after joining
- Each thread should have its own "Timer" object.
- Do I want the number of CS requests to be randomized?
    - How do I have this be consistent? Seed it?




