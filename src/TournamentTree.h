#ifndef TOURNAMENT_TREE_H
#define TOURNAMENT_TREE_H

#include <atomic>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "Lock.h"

using std::atomic;
using std::vector;
using std::unordered_map;

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
    unordered_map<int, int> id_map; // {myid, index}

public:
    void lock(const int myid);
    void unlock(const int myid);
    int pickFlagIndex (); // FIXME: Put back in private after testing
};

/**
 * @brief 
 * 
 */
class TournamentTree : public Lock {
private:
    const int n; // Number of threads
    const int numNodes; // Number of nodes in the tree
    PL *PLArray;


public:
    TournamentTree(const int num);
    void lock(const int myid) override;
    void unlock(const int myid) override;
    int nextLockIndex(int currIndex);
    vector<int> getTopToBottomOrder(const int myid);
    int getPseudoNodeIndex (const int myid);
};

#endif // TOURNAMENT_TREE_H