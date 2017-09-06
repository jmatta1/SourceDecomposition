#!/usr/bin/python
"""The start of the recursive segmentation control script. This script attempts
to find the best segmentation scheme of walls that are 50 feet from the sides
of the PROSPECT AD"""

from librecseg.utility import parse_and_validate_cmd_line
from librecseg import tree_builder as tb
from librecseg import optimizer as opt


def main(num_segments, num_threads, data_dir):
    """The primary function that sets up and runs the decomposition and
    minimization

    Parameters
    ----------
    num_segments : int
        The number of segments per axis at the lowest level of division
    num_threads : int
        The number of threads that can be used in execution of decomps
    data_dir : str
        The path to the directory with the needed input files
    """
    tree_builder = tb.TreeBuilder(num_segments)
    wall_list = tree_builder.get_top_panels()
    decomp_optimizer = opt.DecompOptimizer(num_threads, data_dir)
    decomp_optimizer.perform_optimization(wall_list)


if __name__ == "__main__":
    main(*parse_and_validate_cmd_line())
