#include "TournamentTree.h"
#include <iostream>

using namespace std;


// PL Implementation
/** 
 * @brief Lock method for Petersons Lock.
 * First picks a flag (announce interest) and then yields to other process (victim is me).
 * Will be granted lock if other process is not interested (other flag is false),
 * or if curr process is not the victim. 
 * 
 * @param myid 
 * 
 */
void PL::lock(const int myid) {
    int index = pickFlagIndex();

    if (index == 0) {
        A.myid = myid;
    }
    else {
        B.myid = myid;
    }

    victim = index; 
    while(flag[1-index] && victim == index); // Waits until the other process is not interested or they yield
}

/**
 * @brief Unlock method. Uses a Node to determine which index myid is mapped to.
 * 
 * @param myid 
 */
void PL::unlock(const int myid) {
    int index = (A.myid == myid) ? 0 : 1; // Get the index of the flag that was locked associated with myid
    flag[index] = false;
}

/**
 * @brief Atomic Function used to pick some flag index (that is false)
 * 
 * @return int - index of flag that was taken
 */
int PL::pickFlagIndex () {
    bool compare1 = false;
    bool compare2 = false;
    if (!flag[0].compare_exchange_strong(compare1, true)) { 
        if (!flag[1].compare_exchange_strong(compare2, true)) {
            std::cout << "ERROR in pickFlagIndex()! Both flags are true! Exiting..." << std::endl;
            exit(EXIT_FAILURE); // Shouldn't be possible to fail twice, means a third process attempted to lock
        }
        return 1;
    }
    return 0;
}


// Tournament Tree Implementation -------------------------------------------

/**
 * @brief Construct a new Tournament Tree:: Tournament Tree object
 * 
 * @param num 
 */
TournamentTree::TournamentTree (const int num)
    : n{num}
    , numNodes{(n % 2 == 0) ? n - 1: n} // If n is even, numNodes = n - 1, else p = numNodes.
    , PLArray{new PL[numNodes]} // Might be dangerous to depend on a previous value, but will follow order of member initialization
    // in class declaration (which is the order of the members in this case)
{
}

/**
 * @brief Destroy the Tournament Tree:: Tournament Tree object
 * Deletes and frees up the PLArray.
 */
TournamentTree::~TournamentTree () {
    delete[] PLArray;
}

/**
 * @brief Locking method for TournamentTree lock. 
 * 
 * @param myid 
 * @note The threads are leaf nodes, but have their own thread id. To find what node index they would have
 * if following the structured order of a binary tree, we add n to myid.
 */
void TournamentTree::lock (const int myid) {
    int lockIndex = getPseudoNodeIndex(myid); // Gets the imaginary node index for the thread

    if (nextLockIndex(lockIndex) >= numNodes) {
        std::cout << "ERROR in lock()! lockIndex is out of bounds! Exiting..." << std::endl;
        exit(EXIT_FAILURE);
    }

    while (true) {
        lockIndex = nextLockIndex(lockIndex); // Find the next lock index (parent lock)
        PLArray[lockIndex].lock(myid); // Compete for the current lock
        // cout << "Thread " << myid << " has won the " << lockIndex << "'s lock." << endl; // FIXME: Remove the debug statement
        if (lockIndex <= 0) {
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
    vector<int> order = getTopToBottomOrder(getPseudoNodeIndex(myid));
    for (auto const &i : order) {
        // cout << "Thread " << myid << " unlocking node " << i << endl; // FIXME: Remove the debug statement 
        PLArray[i].unlock(myid); // Unlocks the node in order 
    }
}

/** 
 * @brief Gets the next lock index, aka the parent lock.
 * Uses the function floor{(n-1)/2} .
 * @param currIndex 
 * @return int - the parent/next lock index 
 */
int TournamentTree::nextLockIndex (int currIndex) {
    return ((currIndex - 1) / 2); // Equivalent to floor of (n-1)/2 (because int division)
}

/** 
 * @brief Returns the top to bottom order from some node index.
 * 
 * @param currIndex
 * @return vector<int> - the order from top to bottom for lock
 */
vector<int> TournamentTree::getTopToBottomOrder (int currIndex) {
    // Get the order bottom to up from some node index
    vector<int> order; // FIXME: Can pre-allocate logn elements for better performance

    while (currIndex != 0) {
        currIndex = nextLockIndex(currIndex);
        order.push_back(currIndex); 
    }
    // Reverse the vector to get the order from top to bottom
    reverse(order.begin(), order.end()); 
    return order;
}

/**
 * @brief Gets the node index of some thread to assign the thread to a leaf node.
 * 
 * @param myid 
 * @return int 
 */
int TournamentTree::getPseudoNodeIndex (const int myid) {
    return myid + numNodes;
}
