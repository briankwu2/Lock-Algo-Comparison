#include <iostream>
#include <thread>
#include <vector>
using namespace std;

/*
Obviously has pretty bad issues with data races/atomicity
Meant as a really brief introduction to just have threads run

What did I learn from this simple exercise?
- Shared Resources must be passed using a pointer or ref() function.
- To have a vector of threads, you must use emplace_back() to add threads to the vector
- OR, you can use push_back() to add a thread to the vector, but you must use move() to move the thread into the vector
- You can't use push_back() to add a thread to the vector without move() because threads are not copyable
- You can use emplace_back() because it creates a thread in place, so no copying is necessary
- You can use join() to wait for a thread to finish
*/
void increment(int myid, int *counter) {
    cout << "Thread " << myid << " started" << endl;
    for (int i = 0; i < 1000; i++) {
        (*counter)++;
        cout << "Thread " << myid << " incremented counter to " << *counter << endl;
    }
    cout << "Thread " << myid << " ended" << endl;


}

int main(int argc, char const *argv[])
{
    vector<thread> t; // Create an empty vector of 10 thread objects
    int counter = 0;

    for (int i = 0; i < 10; i++) {
        t.emplace_back(thread(increment, i, &counter)); 
    }

    for (int i = 0; i < 10; i++) {
        t[i].join();
    }
    
    return 0;
}
