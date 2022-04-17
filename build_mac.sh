#!/bin/bash

export CC=gcc-11
export CXX=g++-11
build_yaml() {
    (
        cd yaml/yaml-cpp
        rm -Rf build
        if [ ! -d build ] ; then
            mkdir build
        fi
        cd build
        cmake ..
        make
    )
}

build_yaml
make
