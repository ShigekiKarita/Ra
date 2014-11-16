#!/bin/sh

if [ ! -e build ]; then
    mkdir build
fi

projectName=`grep project CMakeLists.txt | sed -E 's/(project\()(.*)(\))/\2/'`

cd build && cmake .. && make && ./$projectName && cd ..
