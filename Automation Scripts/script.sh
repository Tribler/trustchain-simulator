#!/bin/bash
clear

cd omnetpp-5.4.1/samples/Simulation1/out
rm -r clang-release
rm -r gcc-debug
rm -r gcc-release
cd ..
make
chmod 755 Simulation1

cd ../../../
sbatch clusterJob.job $1
squeue
