#!/bin/bash
cd /home/mpliashko/Desktop/DoodleJump
cmake -B build-linux -DENABLE_ASAN=ON
cmake --build build-linux -- -j
cd build-linux
./DoodleJump >/dev/null 2>&1 &
PID=$!
sleep 7
kill -TERM $PID 2>/dev/null || true
sleep 2
