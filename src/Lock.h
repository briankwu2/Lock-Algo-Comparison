// Not much to see here
// Purely abstract Lock class for use in
// modularizing the main code
#ifndef LOCK_H
#define LOCK_H

class Lock {
public:
    virtual void lock(const int) = 0;
    virtual void unlock(const int) = 0;
};

#endif // LOCK_H