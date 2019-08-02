#!/bin/bash
clear

if [ -f slurm* ]
then
  rm -I slurm*
fi

cd ./result

if [ ! -f SimulationsResultExtractor.class ];
then
    echo "Simulation result extractor compiledX"
    javac SimulationsResultExtractor.java
fi


if [ -f $1 ]
then
    rm output.txt
    rm outputVerbose.txt
    java SimulationsResultExtractor $1 > outputVerbose.txt
    rm $1

    if [ -f $1Log ]
    then
      rm $1Log
    fi

    echo "Data from $1 converted to output.txt and outputVerbose.txt:"
    cat output.txt
else
    echo "The $1 file not exists."
fi
