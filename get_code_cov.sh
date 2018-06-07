#!/bin/bash
for filename in `find . | egrep '\.gcno'`; 
do 
  gcov $filename; 
done
