#!/usr/bin/python

import sys, random

row = int(sys.argv[1])
col = int(sys.argv[2])
n   = int(sys.argv[3])

print str(row) + " " + str(col)

for _ in xrange(n):
    #generate 'i j number'
    i = random.randint(1, row)
    j = random.randint(1, col)
    val = random.random() + 1.0
    print "%d %d %f" % (i, j, val)
    
