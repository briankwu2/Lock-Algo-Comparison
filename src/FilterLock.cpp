#include <vector>
#include <thread>
#include <iostream>
#include <atomic>

using namespace std;

// Class Declarations
class GPL;
class FlagFilterLock;
class LevelFilterLock;

// Function Prototypes
void increment(int &, const int);


int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}

/**
 * @brief Implements the flag type of filter lock.
 * @note This should work the same as the two process Peterson's Lock!
 * FIXME:Test to make sure
 */
class GPL {

    private:
    vector<atomic<bool>> flags;
    atomic<int> victim; 

    public:

    GPL(int n) {
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
    void lock(const int myid) {
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
    void unlock(const int myid) {
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
    bool otherFlagExists(const int myid) {
        for (int i = 0; i < flags.size(); i++) {
            if (myid == i) // Ignore my own flag 
                continue;
            if (flags[i] == true) {
                return true;
            }
        }
        return false;
    }


};

class FlagFilterLock {

    private:
    vector<GPL> level; // pointer to a GPL array


    public:
    FlagFilterLock(int n) {
        vector<GPL> level(n, GPL(n)); // Creates a vector of n levels (GPL)
    }

    void lock() {
        for (int i = 1; i < n; i++) { // 1 to n-1
            level[i].lock();
        }
    }


    void unlock() {
        for (int i = n-1; i > 0; i--) { // n-1 to 1
            level[i].unlock();
        }
    }

};


/**
 * @brief Implements the level based type of filter lock.
 * 
 */
class LevelFilterLock {

    private: 

    public:
    void lock() {

    }

    void unlock() {

    }

};