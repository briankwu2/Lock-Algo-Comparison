#include "FilterLock.h"

using namespace std;

/**
 * @brief Implements the flag type of filter lock.
 * @note This should work the same as the two process Peterson's Lock!
 */
GPL::GPL(int num) 
    : n{num}
    , flags{new atomic<bool>[n]} 
{
}


 /**
 * @brief Public Lock method for GPL.
 * Spins until process p is not the victim OR no other processes are interested
 * 
 *    
 * @param myid 
 */
void GPL::lock(const int myid) {
    flags[myid] = true;
    victim = myid;
    while (otherFlagExists(myid) && victim == myid);
}
/**
 * @brief Public Unlock method for GPL
 * 
 * 
 * @param myid 
 */
void GPL::unlock(const int myid) {
    flags[myid] = false;
}
/**
 * @brief Finds if other flags exists that are not my flag
 * 
 * @param myid 
 * @returns 
 * True: if other flag is true
 * False: if all other flags are false
 */
bool GPL::otherFlagExists(const int myid) {
    for (int i = 0; i < n; i++) {
        if (myid == i) // Ignore my own flag 
            continue;
        if (flags[i] == true) {
            return true;
        }
    }
    return false;
}

// ----------------------------
/**
 * @brief Construct a new Flag Filter Lock:: Flag Filter Lock object
 * 
 * @param n // Number of Threads that will compete for this lock object. 
 */
FlagFilterLock::FlagFilterLock(int num)
    : n{num} 
{
    level.reserve(n); // Pre-allocates space for n number of GPL pointers.
    for (int i = 0; i < n; i++) {

        level.push_back(new GPL(n)); // Creates a pointer and pushes it into the vector
    }
}

/**
 * @brief The lock method for a FlagFilterLock.
 * Calls the GPL Lock Method for every level
 * @param myid Thread ID
 */
void FlagFilterLock::lock(const int myid) {
    for (int i = 1; i < n; i++) { // 1 to n-1
        (*level[i]).lock(myid);
    }
} 

/**
 * @brief The unlock method for a FlagFilterLock.
 * Releases by unlocking each level. 
 * 
 * @param myid Thread ID
 */
void FlagFilterLock::unlock(const int myid) {
    for (int i = n-1; i > 0; i--) { // n-1 to 1
        (*level[i]).unlock(myid);
    }
}


// LevelFilterLock Implementation --------------------------
LevelFilterLock::LevelFilterLock(int num)
    : n{num}
    , level{new atomic<int>[n]}
    , victim{new atomic<int>[n]}
{
}

/**
 * @brief Lock method for LevelFilterLock.
 * Contests for the lock through each level.
 * Claims a spot on the level, and gets through if either:
 * 1. There is no other processes above or equal to it contesting the lock.
 * 2. They are not the victim.
 * 
 * @param myid 
 */
void LevelFilterLock::lock(const int myid) {
    for (int i = 1; i < n - 1; i++) {
        level[myid] = i;
        victim[i] = myid;
        while(existsHigherProcess(myid, i) && victim[i] == myid);
    }
    
}

/**
 * @brief Unlock method for the LevelFilterLock.
 * No longer contesting for lock, setting interest to 0.
 * @param myid Process/Thread ID 
 */
void LevelFilterLock::unlock(const int myid) {
    level[myid] = 0; 
}

/**
 * @brief Finds if there is any other process that is
 * also contesting the lock in a higher level 
 * 
 * @param myid Process/Thread ID 
 * @param currLevel The level to compare other processes against 
 * @returns 
 * True: if there is some process that is in a higher or equal level
 * False: if all other processes are in a lower level 
 */
bool LevelFilterLock::existsHigherProcess(const int myid, const int currLevel) {
    for (int i = 0; i < n; i++) {
        if (myid == i) // Ignore my own flag 
            continue;
        if (level[i] >= currLevel) {
            return true;
        }
    }
    return false;
}