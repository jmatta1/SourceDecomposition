#!/usr/bin/python
"""This files contains the primary code for the position decomposition weight
calculations"""

import sys
from libpd import detector as dt
from libpd import geom_base as gb


def main():
    """Primary entrypoint for the weight calculation code

    Parameters
    ----------
    sys.argv[1] : str
        path to the list of NaI center points
    sys.argv[2] : str
        number of cores to use for the calculation
    """
    if len(sys.argv) != 3:
        print USAGE.format(sys.argv[0])
        sys.exit()
    temp = dt.read_positions(sys.argv[1])
    for i in range(3):
        print "{0:d},".format(i), min([x[2][i] for x in temp])
        print "{0:d},".format(i), max([x[2][i] for x in temp])
    detectors = dt.make_nai_list(dt.read_positions(sys.argv[1]))
    num_cores = int(sys.argv[2])
    source = set_up_sources()


def set_up_sources():
    """This function is what is used to generate the list of sources that the
    weights are calculated for, it then returns the list of source objects
    necessary to perform the weight calculations

    Returns
    -------
    src_list : list
        the list of source objects
    """
    src_list = []
    src_list.extend(make_cube_wall_sources())
    return src_list


def make_cube_wall_sources():
    """This function generates the sources that represent "walls" around the
    AD1 and position scan area

    Returns
    -------
    wall_list : list
        list of square surface objects located at "Effective" walls
    """
    # creating a rotation matrix without adding rotations makes it just the
    # identity matrix
    no_rotate = gb.Rotation()
    # make the wall source that is the Rx pool wall
    # make the wall source that is the MIF room wall
    # make the wall source that is the floor
    # make the wall source that is the ceiling
    # make the wall source that covers the area away from the rollup door
    # make the wall source that covers the area towards the rollup door


USAGE = """Usage:
    {0:s} <Path To NaI Center Points File> <Number of Cores>
"""

if __name__ == "__main__":
    main()
