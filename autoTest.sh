#!/bin/bash

./searchProgram sampleData.dat < testUserInput.txt > sampleTestOutput.txt
diff sampleTestOutput.txt sampleData_testUserInput_result.txt > differences.txt
