#!/bin/sh
tar -xvf cudd-3.0.0.tar.gz
cd cudd-3.0.0/
./configure
make
make check
cd ..
