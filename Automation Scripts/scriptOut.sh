#!/bin/bash
clear

cd omnetpp-5.4.1/samples/Simulation1/results
rm General-*
rm Net*

cd ..
rm -I slurm*

cd ../../../result
rm output.txt
rm outputVerbose.txt
#javac SimulationsResultExtractor.java 
java SimulationsResultExtractor $1 > outputVerbose.txt
rm $1

echo "Data from $1 converted to output.txt:"
cat output.txt
