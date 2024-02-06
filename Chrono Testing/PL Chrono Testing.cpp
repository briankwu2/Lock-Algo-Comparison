#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include "TimerWrapper.h"

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

PL lock;

void increment (int &counter, const int myid, Timer &timer) {
    double timeElapsed;
    int predCount;
    for (int i = 0; i < 100; i++)
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



int main(int argc, char const *argv[])
{
    int counter = 0;
    Timer timer1;
    Timer timer2;

    thread t1(increment, ref(counter), 0, ref(timer1));
    thread t2(increment, ref(counter), 1, ref(timer2));

    t1.join();
    t2.join();

    cout << "All Done!" << endl;
    return 0;
}
