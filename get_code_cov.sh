#!/bin/bash
for filename in `find . | egrep '\.gcno'`; 
do 
  gcov -n -o $filename > /dev/null; 
done
