#! /usr/bin/env python

import os
import sys

def get_next_line(dimension, target_file):
    while True:
        line = target_file.readline()
        if not line:
            return ''
        parts = line.split()
        if ((len(parts) > 0) and (parts[0] == 'Runtime')):
            continue
        if (len(parts) == dimension):
            return line


if (len(sys.argv) < 4):
    print 'Usage: ./check.py <grid_dimension> <validation_file> <output_file>'
    sys.exit()

dimension  = int(sys.argv[1])
val_file_name = sys.argv[2]
out_file_name = sys.argv[3]

val_file = open(val_file_name, 'r')
out_file = open(out_file_name, 'r')

correct = 1

for i in range(dimension):
    val_line = get_next_line(dimension, val_file)
    out_line = get_next_line(dimension, out_file)
    if (val_line == ''):
        print 'Validation file contains fewer than expected lines'
        sys.exit(0)
    if (out_line == ''):
        print 'Output file contains fewer than expected lines'
        sys.exit(0)

    val_parts = val_line.split()
    out_parts = out_line.split()
    for j in range(dimension):
        val_value = float(val_parts[j])
        out_value = float(out_parts[j])

        if ( (out_value < val_value - 0.0001) or (out_value > val_value + 0.0001) ):
            print 'Mismatch at [' + str(i) + '][' + str(j) + ']: expected=' + val_parts[j] + ', observed=' + out_parts[j]
            correct = 0

if (correct == 1):
    print 'Output matches all expected values.'
