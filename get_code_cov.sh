#!/bin/bash
for filename in `find . | egrep '\.gcda'`; 
do 
  gcov -n -o . $filename > /dev/null; 
done
