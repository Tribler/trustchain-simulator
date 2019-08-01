#!/bin/bash
clear

cd omnetpp-5.5.1/samples/Simulation1/results
rm General-*
rm Net*

cd ../../../../
rm -I slurm*

cd ./result

if [ ! -f SimulationsResultExtractor.class ];
then
    echo "Simulation result extractor compiled"
    javac SimulationsResultExtractor.java
fi


if [ -f $1 ]
then
    rm output.txt
    rm outputVerbose.txt
    java SimulationsResultExtractor $1 > outputVerbose.txt
    rm $1

    echo "Data from $1 converted to output.txt and outputVerbose.txt:"
    cat output.txt
else
    echo "The $1 file not exists."
fi
