#include "../src/TournamentTree.h"
#include <iostream>
#include <thread>

using namespace std;


void increment (int &counter, PL &lock, int id) {
    for (int i = 0; i < 1000000; i++) {
        lock.lock(id);
        counter++;
        cout << "Thread " << id << " Counter: " << counter << endl;
        lock.unlock(id);
    }
}

int main(int argc, char const *argv[])
{
    // // Test the nextLockIndex function
    // // Loop through 100 indexes to check if the nextLockIndex function is working
    // for (int i = 0; i < 100; i++) {
    //     int nextIndex = tourTree.nextLockIndex(i);
    //     std::cout << "Index: " << i << " Next Index: " << nextIndex << std::endl;
    // }

    // Next test the getTopToBottomOrder with 100 indexes as well.
    // for (int i = 0; i < 100; i++) {
    //     std::vector<int> order = tourTree.getTopToBottomOrder(i);
    //     std::cout << "Index: " << i << " Order: ";
    //     for (int j = 0; j < order.size(); j++) {
    //         std::cout << order[j] << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // // Test the lock and unlock functions
    // // Lock and unlock 100 times
    // for (int i = 0; i < 100; i++) {
    //     tourTree.lock(0);
    //     std::cout << "Locking: " << i << std::endl;
    //     tourTree.unlock(0);
    //     std::cout << "Unlocking: " << i << std::endl;
    // }


    // Suspect that pickFlagIndex() is not working correctly.
    // Try to just have 2 processes and see if there is a possible deadlock.
    PL pl;
    // Create 2 threads that run the lock function of pl
    int counter = 0;
    thread t1(increment, ref(counter), ref(pl), 3);
    thread t2(increment, ref(counter), ref(pl), 4);

    t1.join();
    t2.join();


    return 0;
}
