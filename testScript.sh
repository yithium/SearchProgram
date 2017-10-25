#!/bin/bash

INPUT1=$1
MAKEPROGRAM=searchProgram$INPUT1


make $MAKEPROGRAM
./autoTest.sh


