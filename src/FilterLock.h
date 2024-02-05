#ifndef FILTER_LOCK_H
#define FILTER_LOCK_H

#include <iostream>
#include <vector>
#include <thread>
#include <atomic> 
#include "Lock.h"

using namespace std;

// Class Declarations
class GPL {

private:
    atomic<bool> *flags; // Pointer to some array of atomic flags
    atomic<int> victim; 
    const int n; // number of threads/processes 

public:
    GPL(int n);

    void lock(const int myid);
    void unlock(const int myid);
    bool otherFlagExists(const int myid);
};

class FlagFilterLock : public Lock {

private:
    vector<GPL*> level; // pointer to a GPL array
    const int n; // Number of Levels

public:
    FlagFilterLock(int n);
    void lock(const int myid) override;
    void unlock(const int myid) override;

};

class LevelFilterLock;

#endif // FILTER_LOCK_H