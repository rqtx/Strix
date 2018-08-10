#!/bin/bash

rm -R build

cmake -H. -Bbuild

cmake --build build -- -j3
