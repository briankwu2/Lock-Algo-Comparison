#ifndef BAKERY_H
#define BAKERY_H
#include "Lock.h"
#include <atomic>
#include <algorithm>

using namespace std;
/**
 * @brief A class based on Lamport's Bakery Algorithm
 * Names of variables and other such are based on class pseudocode.
 * Therefore, variable names and other things are based differently than his original pseudocode.
 */

class Bakery : public Lock {
private:
    const int n; // Number of Processes
    atomic<int> *token; // Array of Tokens of size n
    atomic<bool> *flag; // Array of choosing of size n


public:
    Bakery(const int num);
    void lock(const int myid) override;
    void unlock(const int myid) override;
    bool compareTokens(int *token);

};

#endif // BAKERY_H