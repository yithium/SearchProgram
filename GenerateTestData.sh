#!/bin/bash

rows=1000
cols=500

outputfile=NewTestMatrixData_${rows}_${cols}.dat


userInputNum=10000
userInputMaxElem=10
userInputName=NewUserInput

make inputGen
make generate2D
make encrypt



./generate2D $rows $cols ${outputfile}
./encrypt $outputfile unlock
./inputGen $userInputNum $userInputMaxElem 0 ${userInputName}_sequence.txt
./inputGen $userInputNum $userInputMaxElem 1 ${userInputName}_unordered.txt
./inputGen $userInputNum $userInputMaxElem 2 ${userInputName}_closest.txt