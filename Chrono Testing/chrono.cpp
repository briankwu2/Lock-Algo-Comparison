#include <chrono>
#include <iostream>
#include "TimerWrapper.h"

using namespace std;

int main(int argc, char const *argv[])
{
    Timer t;

    // Records time elapsed (note cout itself takes a lot of time, therefore isn't an accurate estimation
    // of how long it takes to count to 10,000 at all. Just for testing.)
    for (int i = 0; i < 100; i++) {
        cout << "Time Elapsed: " << t.elapsed() << endl;
        int nop = 0;
        for (int j = 0; j < 10000; j++) {
            nop++; // Counts
        }
    }

    cout << "Done!" << endl;
    
    return 0;
}
