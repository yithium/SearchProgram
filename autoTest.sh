#!/bin/bash

./searchProgram sampleData.dat < testUserInput.txt > testResult.txt
diff testResult.txt sampleData_testUserInput_result.txt
