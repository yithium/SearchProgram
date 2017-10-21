#!/bin/bash
./searchProgram output1.dat < testUserInput.bat > testResult.txt
./searchProgram TestGee_encrypted.txt < UserInput1.txt > searchSequenceTest.txt
./searchProgram TestGee_encrypted.txt < UserInputUnordered.txt > searchUnorderedTest.txt
diff testResult.txt Result.txt