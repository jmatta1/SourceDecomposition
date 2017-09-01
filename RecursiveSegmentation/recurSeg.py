#!/usr/bin/python
"""The start of the recursive segmentation control script. This script attempts
to find the best segmentation scheme of walls that are 50 feet from the sides
of the PROSPECT AD"""

from librecseg.utility import parse_and_validate_cmd_line
from librecseg import segment_tree as st


def main(num_segments, num_threads):
    """The primary function that sets up and runs the decomposition and
    minimization

    Parameters
    ----------
    num_segments : int
        The number of segments per axis at the lowest level of division
    num_threads : int
        The number of threads that can be used in execution of decomps
    """
    temp = st.SegmentTree("East_Wall", num_segments)
    temp.print_tree()


if __name__ == "__main__":
    num_segments, num_threads = parse_and_validate_cmd_line()
    # the parameters parsed and were in the correct range
    # therefore we call the main function
    main(num_segments, num_threads)
