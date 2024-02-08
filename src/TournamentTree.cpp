#include "TournamentTree.h"

// PL Implementation
void PL::lock(const int myid) {
    int index = pickFlagIndex();
    victim = index;
    id_map[myid] = index; // To help pass to unlock()
    while(flag[1-index] && victim == index); // Waits until the other process is not interested or they yield
}

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

TournamentTree::TournamentTree (const int num)
    : n{num}
    , PLArray{new PL[n-1]}
{
}

void TournamentTree::lock (const int myid) {

}

void TournamentTree::unlock (const int myid) {
}

int TournamentTree::nextLockIndex (int currIndex) {
    
}