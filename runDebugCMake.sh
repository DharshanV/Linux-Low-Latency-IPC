#!/bin/bash

if [ ! -d "Debug" ]
then
    mkdir Debug
fi

cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make