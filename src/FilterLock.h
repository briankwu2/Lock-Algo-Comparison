#ifndef FILTER_LOCK_H
#define FILTER_LOCK_H

#include <iostream>
#include <vector>
#include <thread>
#include <atomic> 

using namespace std;

// Class Declarations
class GPL {

    private:
    vector<atomic<bool>> flags;
    atomic<int> victim; 

    public:
    GPL(int n);

    void lock(const int myid);
    void unlock(const int myid);
    bool otherFlagExists(const int myid);
};
class FlagFilterLock {

    private:
    vector<GPL> level; // pointer to a GPL array
    int n; // Number of Levels

    public:
    FlagFilterLock(int n);
    void lock(const int myid);
    void unlock(const int myid);

};

class LevelFilterLock;

// Function Prototypes
void increment(int &, const int);



#endif // FILTER_LOCK_H