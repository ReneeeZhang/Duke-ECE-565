#!/bin/bash

mkdir validation/pt
# Run the program and generate validation files
cd rainfall && make clean && make
for NUM_THREADS in {1,2,4,8}
do
    ./rainfall_pt ${NUM_THREADS} 20 0.5 32 ../test/input/sample_32x32.in > ../validation/pt/sample_32_${NUM_THREADS}
    ./rainfall_pt ${NUM_THREADS} 30 0.25 128 ../test/input/sample_128x128.in > ../validation/pt/sample_128_${NUM_THREADS}
    ./rainfall_pt ${NUM_THREADS} 30 0.75 512 ../test/input/sample_512x512.in > ../validation/pt/sample_512_${NUM_THREADS}
    ./rainfall_pt ${NUM_THREADS} 35 0.5 2048 ../test/input/sample_2048x2048.in > ../validation/pt/sample_2048_${NUM_THREADS}
done

# # Use check
# cd ..
for NUM_THREADS in {1,2,4,8}
do
    echo sample_32_${NUM_THREADS}
    ./check.py 32 validation/pt/sample_32_${NUM_THREADS} test/output/sample_32x32.out
    echo sample_128_${NUM_THREADS}
    ./check.py 128 validation/pt/sample_128_${NUM_THREADS} test/output/sample_128x128.out
    echo sample_512_${NUM_THREADS}
    ./check.py 512 validation/pt/sample_512_${NUM_THREADS} test/output/sample_512x512.out
    echo sample_2048_${NUM_THREADS}
    ./check.py 2048 validation/pt/sample_2048_${NUM_THREADS} test/output/sample_2048x2048.out  
done
