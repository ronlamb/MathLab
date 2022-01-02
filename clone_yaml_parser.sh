#!/bin/bash
LIBDIR=yaml/yaml-cpp
git clone https://github.com/jbeder/yaml-cpp/ $LIBDIR
cd $LIBDIR
git checkout tags/yaml-cpp-0.7.0 -b v0.7.0
