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

PL lock;

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
    thread t1(increment, ref(counter), 0);
    thread t2(increment, ref(counter), 1);

    t1.join();
    t2.join();

    return 0;
}
