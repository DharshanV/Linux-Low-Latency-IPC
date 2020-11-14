#!/bin/bash

if [ ! -d "Release" ]
then
    mkdir Release
fi

cd Release
cmake -DCMAKE_BUILD_TYPE=Release ..
make