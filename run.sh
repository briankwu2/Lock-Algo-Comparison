#!/bin/bash
# For debugging with valgrind
# g++-11 -ggdb3 ./src/*.cpp ./src/TimerWrapper.h ./src/dataFunctions.h \ 
#-o ./Locks

echo "Compiling with -O3 and -DNDEBUG flags..."
g++-11 -O3 -DNDEBUG ./src/*.cpp ./src/TimerWrapper.h ./src/dataFunctions.h \
	-o ./LockPerformance
if [ -e "LockPerformance" ]
then
	echo "Compilation Sucessful, running ./LockPerformance"
else
	echo "File not compiled sucessfully, exiting..."
	exit 1
fi

echo "Running LockPerformance now..."
./LockPerformance

echo "Done! Removing ./LockPerformance..."
rm ./LockPerformance



