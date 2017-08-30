#!/usr/bin/python
"""The start of the recursive segmentation control script. This script attempts
to find the best segmentation scheme of walls that are 50 feet from the sides
of the PROSPECT AD"""

import sys

# TODO: Add docstrings

def main(numSegments, numThreads):
    pass


USAGE_STR = """Usage:
    {0:s} <Number of Axial Pieces> <Number of Threads>
"""

NUMSEG_ERR = """  Error:
    Could not parse <Number of Axial Pieces> as a valid integer
    <Number of Axial Pieces> must be an integer greater than or equal to 2

    It describes the number of pieces the walls have been divided into on each
    of the two axes.
"""

NUMTHREAD_ERR = """  Error:
    Could not parse <Number of Threads> as a valid integer
    <Number of Threads> must be an integer greater than or equal to 1

    It is the maximum number of simultaneous decompositions it can perform
"""

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print USAGE_STR.format(sys.argv[0])
        sys.exit()
    # attempt to parse the parameters tell the user and exit if we can't
    numSegments = 0
    try:
        numSegments = int(sys.argv[1])
        if numSegments < 2:
            raise ValueError("<Number of Axial Pieces> is less than 2")
    except:
        print USAGE_STR.format(sys.argv[0])
        print NUMSEG_ERR
        sys.exit()
    numThreads = 0
    try:
        numThreads = int(sys.argv[2])
        if numThreads < 1:
            raise ValueError("<Number of Threads> is less than 1")
    except:
        print USAGE_STR.format(sys.argv[0])
        print NUMTHREAD_ERR
        sys.exit() 
    # the parameters parsed well and were in the correct range
    main(numSegments, numThreads)
