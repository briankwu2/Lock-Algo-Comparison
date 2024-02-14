#include "BakeryLock.h"


/**
 * @brief Construct a new Bakery Lock:: Bakery Lock object
 * Initializes all tokens to 0 and flags to false.
 * @param num 
 */
BakeryLock::BakeryLock(const int num) 
    : n{num}
    , token{new atomic<int>[n]}
    , flag{new atomic<bool>[n]}
{
    for (int i = 0; i < n; i++) {
        token[i] = 0;
        flag[i] = false;
    }

}

/**
 * @brief Destroy the Bakery Lock:: Bakery Lock object
 * Deletes the token and flag arrays.
 */
BakeryLock::~BakeryLock() {
    delete[] token;
    delete[] flag;
}

/**
 * @brief Locking method for the BakeryLock algorithm.
 * Allows critical section for the thread/process that has the lowest token.
 * If two processes have the same token number, let the smaller process id win.
 *  
 * @param myid 
 */
void BakeryLock::lock(const int myid) {
    flag[myid] = true;
    token[myid] = 1 + *max_element(token, token+n); 
    flag[myid] = false;

    for (int i = 0; i < n; i++) {
        while (flag[i]); // If they are choosing a number, then wait
        while(
            token[i] != 0 && // Gain priority over i if: process i has no interest 
            (token[i] < token[myid] || // Process i has a lower token number 
            (token[i] == token[myid] && i < myid)) // Process i and me have same token number but their id is lower
        );
    }
}

/**
 * @brief Release the lock by setting token to 0.
 * 
 * @param myid 
 */
void BakeryLock::unlock(const int myid) {
    token[myid] = 0;
}

