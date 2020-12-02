#!/bin/bash

mkdir validate/performance

cd rainfall && make clean && make
for NUM_THREADS in {1,2,4,8}
do
    ./rainfall_pt ${NUM_THREADS} 50 0.5 4096 ../test/input/measurement_4096x4096.in > ../validation/performance/measuement_4096_${NUM_THREADS}
done 

for NUM_THREADS in {1,2,4,8}
do
    echo validate measuement_4096 with ${NUM_THREADS} thread
    ./check.py 4096 validation/performance/measuement_4096_${NUM_THREADS} test/output/measurement_4096x4096.out 
done

