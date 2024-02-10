#include "../src/TournamentTree.h"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    TournamentTree tourTree(2);

    // // Test the nextLockIndex function
    // // Loop through 100 indexes to check if the nextLockIndex function is working
    // for (int i = 0; i < 100; i++) {
    //     int nextIndex = tourTree.nextLockIndex(i);
    //     std::cout << "Index: " << i << " Next Index: " << nextIndex << std::endl;
    // }

    // Next test the getTopToBottomOrder with 100 indexes as well.
    for (int i = 0; i < 100; i++) {
        std::vector<int> order = tourTree.getTopToBottomOrder(i);
        std::cout << "Index: " << i << " Order: ";
        for (int j = 0; j < order.size(); j++) {
            std::cout << order[j] << " ";
        }
        std::cout << std::endl;
    }

    // // Test the lock and unlock functions
    // // Lock and unlock 100 times
    // for (int i = 0; i < 100; i++) {
    //     tourTree.lock(0);
    //     std::cout << "Locking: " << i << std::endl;
    //     tourTree.unlock(0);
    //     std::cout << "Unlocking: " << i << std::endl;
    // }


   // Test individual locking on a big tree.

    





    return 0;
}
