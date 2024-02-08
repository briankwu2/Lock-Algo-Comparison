#include "TournamentTree.h"

// PL Implementation
/** FIXME: The whole new PL needs to be tested
 * @brief Lock method for Petersons Lock.
 * First picks a flag (announce interest) and then yields to other process (victim is me).
 * Will be granted lock if other process is not interested (other flag is false),
 * or if curr process is not the victim. 
 * 
 * @param myid 
 */
void PL::lock(const int myid) {
    int index = pickFlagIndex();
    victim = index;
    id_map[myid] = index; // To help pass to unlock()
    while(flag[1-index] && victim == index); // Waits until the other process is not interested or they yield
}

/**
 * @brief 
 * 
 * @param myid 
 */
void PL::unlock(const int myid) {
    int index = id_map[myid];
    id_map.erase(myid);
    flag[index] = false;
}

/**
 * @brief Atomic Function used to pick some flag index (that is false)
 * 
 * @return int - index of flag that was taken
 */
int PL::pickFlagIndex () {
    bool compare = false;
    if (!flag[0].compare_exchange_strong(compare, true)) { 
        compare = false;
        if (!flag[1].compare_exchange_strong(compare, true)) {
            //exit(); // Shouldn't be possible to fail twice, means a third process attempted to lock
        }
    }
}

// Tournament Tree Implementation

/**
 * @brief Construct a new Tournament Tree:: Tournament Tree object
 * 
 * @param num 
 */
TournamentTree::TournamentTree (const int num)
    : n{num}
    , PLArray{new PL[n-1]}
{
}

/**
 * @brief Locking method for TournamentTree lock. 
 * 
 * @param myid 
 * @note The threads are leaf nodes, but have their own thread id. To find what node index they would have
 * if following the structured order of a binary tree, we add n - 1 to myid.
 */
void TournamentTree::lock (const int myid) {
    int lockIndex = nextLockIndex(myid + n - 1); // Finds the first lock associated with the thread id
    while (true) {
        PLArray[lockIndex].lock(myid); // Compete for the current lock
        lockIndex = nextLockIndex(lockIndex); // Find the next lock index (parent lock)
        if (lockIndex == 0) {
            break; // Process now gets the final lock for critical section
        }
    }
}

/**
 * @brief Unlocks from root node to leaf (of thread). 
 * Currently implements the top-to-bottom type unlocking.
 * A little bit more expensive by logn. 
 * @param myid 
 */
void TournamentTree::unlock (const int myid) {
    vector<int> order = getTopToBottomOrder(myid);
    for (int i = order.size() - 1; i >= 0; i--) {
        PLArray[i].unlock(myid);
    }
}

/** FIXME: Test this function
 * @brief Gets the next lock index, aka the parent lock.
 * Uses the function floor{(n-1)/2} .
 * @param currIndex 
 * @return int - the parent/next lock index 
 */
int TournamentTree::nextLockIndex (int currIndex) {
    return ((currIndex - 1) / 2); // Equivalent to floor of (n-1)/2 (because int division)
}

/** FIXME: Test this function
 * @brief 
 * 
 * @param myid 
 * @return vector<int> - the order from top to bottom for lock
 */
vector<int> TournamentTree::getTopToBottomOrder (const int myid) {
    // Get the order bottom to up
    int tempIndex = myid + n - 1;
    vector<int> order; // FIXME: Can pre-allocate logn elements for better performance
    while (tempIndex != 0) {
        tempIndex = nextLockIndex(tempIndex);
        order.push_back(tempIndex); 
    }

    // Reverse the vector to get the order from top to bottom
    reverse(order.begin(), order.end());

    return order;
}
