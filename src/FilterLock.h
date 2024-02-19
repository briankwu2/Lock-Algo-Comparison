#ifndef FILTER_LOCK_H
#define FILTER_LOCK_H

#include <iostream>
#include <vector>
#include <thread>
#include <atomic> 
#include "Lock.h"

using std::atomic;
using std::vector;

// Class Declarations

class GPL {

private:
    const int n; // number of threads/processes 
    atomic<bool> *flags; // Pointer to some array of atomic flags
    atomic<int> victim; 

public:
    GPL(int n);
    ~GPL();
    void lock(const int myid);
    void unlock(const int myid);
    bool otherFlagExists(const int myid);
};

class FlagFilterLock : public Lock {

private:
    const int n; // Number of Levels
    vector<GPL*> level; // pointer to a GPL array

public:
    FlagFilterLock(int n);
    ~FlagFilterLock();
    void lock(const int myid) override;
    void unlock(const int myid) override;

};

class LevelFilterLock : public Lock {
private:
    const int n; // Number of processes/threads
    atomic<int> *level; // Level Array of size n
    atomic<int> *victim; // Victim Array of size n

public:
    LevelFilterLock(int n);
    ~LevelFilterLock();
    void lock(const int myid) override;
    void unlock(const int myid) override;
    bool existsHigherProcess(const int myid, const int currLevel);

};

#endif // FILTER_LOCK_H