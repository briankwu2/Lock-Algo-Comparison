#ifndef TOURNAMENT_TREE_H
#define TOURNAMENT_TREE_H

#include <atomic>
#include <vector>
#include <algorithm>
#include "Lock.h"

using std::atomic;
using std::vector;

/**
 * @brief A strange way of recording the index of the flag and the id that is associated with it.
 * Implemented this way after having trouble with the Petersons Lock that
 * was using a hashmap/unordered_map to store the id and the flag index. 
 * There was concurrency issues with the unordered_map as there was undefined behavior if
 * the id was removed from the map while the other process was trying to access it regardless
 * of what the expected behavior was.
 * 
 */
struct Record {
public:
    const int index;
    int myid;

    Record (int index)
        : index(index){
    }
};

/**
 * @brief Basic Idea of Petersons Lock:
 *  If one process is interested, then turn flag[myid] true.
 *  Also yield to the other process first and set victim = myid
 *  To get the lock, the other process must be not trying to get the lock or the victim is not me
 *  This PL is modified for Tournament Tree, it needs to associate some myid with the index for flags.
 *  It must also be able to remove and add ids at will.
 *  At most it will only have 2 ids store in the hash map so it is very little overhead.
 */
class PL {
private:
    // These are shared variables!
    atomic<bool> flag[2] = {false, false};
    atomic<int> victim; 
    Record A{0}; // Permanently initialized with flag index 0
    Record B{1}; // Permanently initialized with flag index 1
    int pickFlagIndex (); 

public:
    void lock(const int myid);
    void unlock(const int myid);
};

/**
 * @brief Tournament Tree Lock that implements a modified version of the Peterson's Lock for each lock node.
 * 
 */
class TournamentTree : public Lock {
private:
    const int n; // Number of threads
    const int numNodes; // Number of nodes in the tree
    PL *PLArray; // Array of Peterson's Locks

public:
    TournamentTree(const int num);
    ~TournamentTree();
    void lock(const int myid) override;
    void unlock(const int myid) override;
    int nextLockIndex(int currIndex);
    vector<int> getTopToBottomOrder(const int myid);
    int getPseudoNodeIndex (const int myid);
};

#endif // TOURNAMENT_TREE_H