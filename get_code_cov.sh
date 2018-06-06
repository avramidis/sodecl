#!/bin/bash
for filename in `find src/sodecl/. | egrep '\.hpp'`; 
do 
  gcov -n -o . $filename > /dev/null; 
done
