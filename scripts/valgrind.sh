#!/bin/sh
./run.sh build && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind-out.txt ./build/bin/ksh
