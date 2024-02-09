#include <iostream>
#include <atomic>
#include <vector>
#include <thread>

#define NUM_INCREMENTS 1000000

using namespace std;

void incrementAtomic (atomic<int> &a) {
    for (int i = 0; i < NUM_INCREMENTS; i++ ) {
        // a += 2; // Should be Atomic
        a = a + 2; // Shouldn't Be Atomic
    }

}

int main(int argc, char const *argv[])
{

    vector<thread> threads;
    atomic<int> counter = 0;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back(thread(incrementAtomic, ref(counter)));
    }

    for (int i = 0; i< 10; i++) {
        threads[i].join();
    }

    cout << "Expected Counter Value: " << 2* 10 * NUM_INCREMENTS << endl;
    cout << "Counter Value is: " << counter << endl;

    

    return 0;
}
