#include "../src/dataFunctions.h"
#include "../src/TimerWrapper.h"
#include "../src/BakeryLock.h"
#include <thread>
#include <iostream>

using namespace std;


/**
 * @brief Demonstrates that the ThreadParameters struct can be used to pass
 * parameters to a function.
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char const *argv[])
{

    int counter = 0;
    double timeElapsed  = 0;
    Timer tpTimer;
    Timer sysTimer;
    BakeryLock lock(2);
    const int myid1 = 0;

    ThreadParameters tp1(myid1, timeElapsed, counter, tpTimer, sysTimer, &lock);
    incrDebug(tp1);

    cout << "Counter: " << counter << endl;


    return 0;
}
