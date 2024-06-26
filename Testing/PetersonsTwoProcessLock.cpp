#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
#include "../src/FilterLock.h"
using namespace std;

class PL {

/*
Basic Idea of Petersons Lock:
If one process is interested, then turn flag[myid] true.
Also yield to the other process first and set victim = myid
To get the lock, the other process must be not trying to get the lock or the victim is not me

EDIT: File has been edited to run FlagFilterLock instead as a test run.
*/
private:
    // These are shared variables!
    atomic<bool> flag[2] = {false, false};
    atomic<int> victim; 

public:
    void lock(const int myid) {
        flag[myid] = true;
        victim = myid;
        while(flag[1-myid] && victim == myid) {}// Waits until the other process is not interested or they yield
    }

    void unlock(const int myid) {
        flag[myid] = false;
    }

};

FlagFilterLock lock(5);

void increment (int &counter, const int myid) {
    for (int i = 0; i < 1000; i++)
    {
        lock.lock(myid); 
        counter++;
        cout << "Thread " << myid << ": " << counter << endl; 
        lock.unlock(myid);
    }
}



int main(int argc, char const *argv[])
{
    int counter = 0;
    vector<thread> threads; 

    for (int i = 0; i < 5; i++) {
        threads.emplace_back(increment, ref(counter), i); // In-Place instantiation of thread. (Cannot move/copy)
    }

    for (int i = 0; i < 5; i++) {
        threads[i].join();
    }

    return 0;
}
