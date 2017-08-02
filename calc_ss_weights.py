#!/usr/bin/python
"""This files contains the primary code for the source summation weight
calculations, i.e. it uses the infrastructure from position decomposition
to calculate the weight of each source on the AD1 'patches' """

import sys
import numpy as np
from libpd import detector as dt
from libpd import geom_base as gb
from libpd import flat_sources as fs
from libpd import shell_sources as ss
from libpd import weight_calc as wc

def main():
    """Primary entrypoint for the weight calculation code

    Parameters
    ----------
    sys.argv[1] : str
        path to the list of AD1 patches, normals, and sizes
    sys.argv[2] : str
        number of cores to use for the calculation
    sys.argv[3] : str
        name of the output file
    """
    if len(sys.argv) != 4:
        print USAGE.format(sys.argv[0])
        sys.exit()
    detectors = dt.read_patches(sys.argv[1])
    num_cores = int(sys.argv[2])
    sources = set_up_sources()
    weights = wc.calculate_weights(detectors, sources, num_cores)
    out_file = open(sys.argv[3], 'w')
    fmt_str = "{0:d}, {1:d}, {2:s}, {3:10.8e}\n"
    for elem in weights:
        out_file.write(fmt_str.format(*elem))
    out_file.close()

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
    #src_list.extend(make_hot_patches())
    #src_list.extend(make_vertical_cylinders())
    #src_list.extend(make_beamlines())
    return src_list


def make_beamlines():
    """This function generates cylindrical shells that represent the three
    beamlines that we care about

    Returns
    -------
    src_list : list
        the list of source objects
    """
    beam_list = []
    # make HB3
    center = 2.54*np.array([188.0, 232.5, -163.653], dtype=np.float64)
    beam_list.append(ss.RotXaxisCylinder("HB3", center, (15.0, 2.54*418.0), -31.5881))
    # make HB3a
    center = 2.54*np.array([88.5, 48.65, -163.653], dtype=np.float64)
    beam_list.append(ss.RotXaxisCylinder("HB3a", center, (15.0, 2.54*165.0), 28.4119))
    # make HB4
    center = 2.54*np.array([173.5, -210.42, -163.653], dtype=np.float64)
    beam_list.append(ss.RotXaxisCylinder("HB4", center, (30.0, 2.54*770.0), 60.0))
    return beam_list


def make_vertical_cylinders():
    """This function generates cylindrical shells that represent the access
    pipes down to the beam lines

    Returns
    -------
    src_list : list
        the list of source objects
    """
    vert_list = []
    # make the vertical pipe to beamline 3
    center = 2.54*np.array([185.0, 222.0, -81.8265], dtype=np.float64)
    vert_list.append(ss.VertCylinder("HB3_Access", center, (15.0, 2.54*163.653)))
    # make the vertical pipe to beamline 4
    center = 2.54*np.array([192.5, 16.0, -81.8265], dtype=np.float64)
    vert_list.append(ss.VertCylinder("HB4_Access", center, (20.0, 2.54*163.653)))
    return vert_list


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
    center = 2.54*np.array([228.0, 105.0, 11.0], dtype=np.float64)
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
    center = 2.54*np.array([219.0, 69.0, 95.0], dtype=np.float64)
    edge_vec1 = 2.54*np.array([0.0, 81.0, 0.0], dtype=np.float64)
    edge_vec2 = 2.54*np.array([0.0, 0.0, 95.0], dtype=np.float64)
    wall_list.append(fs.Square("Rx_Wall", center, (edge_vec1, edge_vec2),
                               no_rotate))
    # make the wall source that is the MIF room wall
    center = 2.54*np.array([0.0, 69.0, 95.0], dtype=np.float64)
    edge_vec1 = 2.54*np.array([0.0, 81.0, 0.0], dtype=np.float64)
    edge_vec2 = 2.54*np.array([0.0, 0.0, 95.0], dtype=np.float64)
    wall_list.append(fs.Square("MIF_Room_Wall", center, (edge_vec1, edge_vec2),
                               no_rotate))
    # make the wall source that is the floor
    center = 2.54*np.array([109.5, 69.0, 0.0], dtype=np.float64)
    edge_vec1 = 2.54*np.array([0.0, 81.0, 0.0], dtype=np.float64)
    edge_vec2 = 2.54*np.array([109.5, 0.0, 0.0], dtype=np.float64)
    wall_list.append(fs.Square("Floor", center, (edge_vec1, edge_vec2),
                               no_rotate))
    # make the wall source that is the ceiling
    center = 2.54*np.array([109.5, 69.0, 190.0], dtype=np.float64)
    edge_vec1 = 2.54*np.array([0.0, 81.0, 0.0], dtype=np.float64)
    edge_vec2 = 2.54*np.array([109.5, 0.0, 0.0], dtype=np.float64)
    wall_list.append(fs.Square("Ceiling", center, (edge_vec1, edge_vec2),
                               no_rotate))
    # make the wall source that covers the area opposite the rollup door (east)
    center = 2.54*np.array([109.5, -12.0, 95.0], dtype=np.float64)
    edge_vec1 = 2.54*np.array([0.0, 0.0, 95.0], dtype=np.float64)
    edge_vec2 = 2.54*np.array([109.5, 0.0, 0.0], dtype=np.float64)
    wall_list.append(fs.Square("East_Wall", center, (edge_vec1, edge_vec2),
                               no_rotate))
    # make the wall source that covers the area towards the rollup door (west)
    center = 2.54*np.array([109.5, 150.0, 95.0], dtype=np.float64)
    edge_vec1 = 2.54*np.array([0.0, 0.0, 95.0], dtype=np.float64)
    edge_vec2 = 2.54*np.array([109.5, 0.0, 0.0], dtype=np.float64)
    wall_list.append(fs.Square("West_Wall", center, (edge_vec1, edge_vec2),
                               no_rotate))
    return wall_list


USAGE = """Usage:
    {0:s} <Path To NaI Center Points File> <Number of Cores> <Ouput File Name>
"""

if __name__ == "__main__":
    main()
