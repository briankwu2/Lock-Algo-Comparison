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
- Note that the <atomic> library is used as a safeguard to ensure that "memory re-ordering" does
not occur, and potentially void mutual exclusion. Where memory ordering is a modern optimization technique
used in most CPUs that may potentially void the order of read and write operations that will guarantee
mutual exclusion.
See https://en.wikipedia.org/wiki/Memory_ordering.
- Therefore the <atomic> library guarantees that the order that key concurrency variables may be stable.
- Otherwise concurrency is still maintained primarily through the lock algorithims, and nothing else significant actually changes.