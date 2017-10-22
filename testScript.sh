#!/bin/bash

INPUT1=$1
MAKEPROGRAM=searchProgram$INPUT1

make $MAKEPROGRAM #searchProgramMinPrint
./autoTest.sh


