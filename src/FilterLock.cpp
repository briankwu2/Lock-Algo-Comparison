#include "FilterLock.h"

using namespace std;

/**
 * @brief Implements the flag type of filter lock.
 * @note This should work the same as the two process Peterson's Lock!
 * FIXME:Test to make sure
 */
GPL::GPL(int n) {
    for (int i = 0; i < n; i++) {
        flags.push_back(false);
    }
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
    for (int i = 0; i < flags.size(); i++) {
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
FlagFilterLock::FlagFilterLock(int n) {
    vector<GPL> level(n, GPL(n)); // Creates a vector of n levels (GPL)
    this->level = level;
    this->n = n;
}

/**
 * @brief The lock method for a FlagFilterLock.
 * Calls the GPL Lock Method for every level
 * @param myid Thread ID
 */
void FlagFilterLock::lock(const int myid) {
    for (int i = 1; i < n; i++) { // 1 to n-1
        level[i].lock(myid);
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
        level[i].unlock(myid);
    }
}



