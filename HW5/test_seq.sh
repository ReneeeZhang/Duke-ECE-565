#!/bin/bash

mkdir validation/seq
# Run the program and generate validation files
cd rainfall && make clean && make
./rainfall_seq 10 0.25 4 ../test/input/sample_4x4.in > ../validation/seq/sample_4
./rainfall_seq 20 0.5 16 ../test/input/sample_16x16.in > ../validation/seq/sample_16
./rainfall_seq 20 0.5 32 ../test/input/sample_32x32.in > ../validation/seq/sample_32
./rainfall_seq 30 0.25 128 ../test/input/sample_128x128.in > ../validation/seq/sample_128
./rainfall_seq 30 0.75 512 ../test/input/sample_512x512.in > ../validation/seq/sample_512
./rainfall_seq 35 0.5 2048 ../test/input/sample_2048x2048.in > ../validation/seq/sample_2048

# Check
cd ..
echo sample_4
./check.py 4 validation/seq/sample_4 test/output/sample_4x4.out
echo sample_16
./check.py 16 validation/seq/sample_16 test/output/sample_16x16.out
echo sample_32
./check.py 32 validation/seq/sample_32 test/output/sample_32x32.out
echo sample_128
./check.py 128 validation/seq/sample_128 test/output/sample_128x128.out
echo sample_512
./check.py 512 validation/seq/sample_512 test/output/sample_512x512.out
echo sample_2048
./check.py 2048 validation/seq/sample_2048 test/output/sample_2048x2048.out
