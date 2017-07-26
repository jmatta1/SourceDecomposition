#!/usr/bin/python
"""This files contains the primary code for the position decomposition weight
calculations"""

import sys
import numpy as np
from libpd import detector as dt
from libpd import geom_base as gb
from libpd import flat_sources as fs
from libpd import weight_calc as wc

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
    detectors = dt.make_nai_list(dt.read_positions(sys.argv[1]))
    num_cores = int(sys.argv[2])
    sources = set_up_sources()
    temp = wc.calculate_weights(detectors, sources, num_cores)
    for elem in temp:
        print elem


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
    src_list.extend(make_hot_patches())
    return src_list


def make_hot_patches():
    """This function generates 2D sources that represent flat hot-spots in
    various places

    Returns
    -------
    patch_list : list
        list of 2D sources that are not the "Effective" walls
    """
    patch_list = []
    # make the source that represents the hot-patch from EF4
    center = 2.54*np.array([228.0, 105.0, 11.0])
    patch_list.append(fs.CircleYZ("EF4_Cover", center, 20.0))
    return patch_list


def make_cube_wall_sources():
    """This function generates the sources that represent "walls" around the
    AD1 and position scan area

    Returns
    -------
    wall_list : list
        list of square surface objects located at "Effective" walls
    """
    wall_list = []
    # creating a rotation matrix without adding rotations makes it just the
    # identity matrix
    no_rotate = gb.Rotation()
    # make the wall source that is the Rx pool wall
    center = 2.54*np.array([219.0, 69.0, 95.0])
    edge_vec1 = 2.54*np.array([0.0, 81.0, 0.0])
    edge_vec2 = 2.54*np.array([0.0, 0.0, 95.0])
    wall_list.append(fs.Square("Rx_Wall", center, (edge_vec1, edge_vec2),
                               no_rotate))
    # make the wall source that is the MIF room wall
    center = 2.54*np.array([0.0, 69.0, 95.0])
    edge_vec1 = 2.54*np.array([0.0, 81.0, 0.0])
    edge_vec2 = 2.54*np.array([0.0, 0.0, 95.0])
    wall_list.append(fs.Square("MIF_Room_Wall", center, (edge_vec1, edge_vec2),
                               no_rotate))
    # make the wall source that is the floor
    center = 2.54*np.array([109.5, 69.0, 0.0])
    edge_vec1 = 2.54*np.array([0.0, 81.0, 0.0])
    edge_vec2 = 2.54*np.array([109.5, 0.0, 0.0])
    wall_list.append(fs.Square("Floor", center, (edge_vec1, edge_vec2),
                               no_rotate))
    # make the wall source that is the ceiling
    center = 2.54*np.array([109.5, 69.0, 190.0])
    edge_vec1 = 2.54*np.array([0.0, 81.0, 0.0])
    edge_vec2 = 2.54*np.array([109.5, 0.0, 0.0])
    wall_list.append(fs.Square("Ceiling", center, (edge_vec1, edge_vec2),
                               no_rotate))
    # make the wall source that covers the area opposite the rollup door (east)
    center = 2.54*np.array([109.5, -12.0, 95.0])
    edge_vec1 = 2.54*np.array([0.0, 0.0, 95.0])
    edge_vec2 = 2.54*np.array([109.5, 0.0, 0.0])
    wall_list.append(fs.Square("East_Wall", center, (edge_vec1, edge_vec2),
                               no_rotate))
    # make the wall source that covers the area towards the rollup door (west)
    center = 2.54*np.array([109.5, 150.0, 95.0])
    edge_vec1 = 2.54*np.array([0.0, 0.0, 95.0])
    edge_vec2 = 2.54*np.array([109.5, 0.0, 0.0])
    wall_list.append(fs.Square("West_Wall", center, (edge_vec1, edge_vec2),
                               no_rotate))
    return wall_list


USAGE = """Usage:
    {0:s} <Path To NaI Center Points File> <Number of Cores>
"""

if __name__ == "__main__":
    main()
