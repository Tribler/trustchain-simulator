#!/bin/bash
#to remove DOS line endind: sed -i -e 's/\r$//' fileName.sh
clear

cd omnetpp-5.5.1/samples/Simulation1/out
rm -r clang-release
rm -r gcc-debug
rm -r gcc-release

cd ../results
rm General-*
rm Net*

cd ..
make clean
make MODE=release -j 8
chmod 755 Simulation1

cd ../../../
sbatch clusterJob.job $1
squeue
