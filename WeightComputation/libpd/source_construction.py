"""This file contains all the functions necessary to build one of the
sterotypical source sets for decomposition"""

import numpy as np
import libpd.geom_base as gb
import libpd.flat_sources as fs
import libpd.shell_sources as ss
import libpd.low_dim_sources as lds

#      [MAX VALUE   , CENTER VALUE,     MIN VALUE]
WALX = [2.54 * 800.0, 2.54 * 136.0, -2.54 * 528.0]
WALY = [2.54 * 729.5, 2.54 * 071.5, -2.54 * 586.5]
WALZ = [2.54 * 725.5, 2.54 * 62.75, -2.54 * 600.0]
WALHW = [(WALX[0]-WALX[2])/2.0, (WALY[0]-WALY[2])/2.0, (WALZ[0]-WALZ[2])/2.0]

WALL_CENTERS = [np.array([WALX[0], WALY[1], WALZ[1]], dtype=np.float64),  # RxWall
                np.array([WALX[2], WALY[1], WALZ[1]], dtype=np.float64),  # MifWall
                np.array([WALX[1], WALY[1], WALZ[0]], dtype=np.float64),  # ceil
                np.array([WALX[1], WALY[1], WALZ[2]], dtype=np.float64),  # floor
                np.array([WALX[1], WALY[0], WALZ[1]], dtype=np.float64),  # west
                np.array([WALX[1], WALY[2], WALZ[0]], dtype=np.float64)]  # east

WALL_EDGES1 = [np.array([0.0, WALHW[1], 0.0], dtype=np.float64),
               np.array([0.0, WALHW[1], 0.0], dtype=np.float64),
               np.array([0.0, WALHW[1], 0.0], dtype=np.float64),
               np.array([0.0, WALHW[1], 0.0], dtype=np.float64),
               np.array([0.0, 0.0, WALHW[2]], dtype=np.float64),
               np.array([0.0, 0.0, WALHW[2]], dtype=np.float64)]

WALL_EDGES2 = [np.array([0.0, 0.0, WALHW[2]], dtype=np.float64), 
               np.array([0.0, 0.0, WALHW[2]], dtype=np.float64),
               np.array([WALHW[0], 0.0, 0.0], dtype=np.float64),
               np.array([WALHW[0], 0.0, 0.0], dtype=np.float64),
               np.array([WALHW[0], 0.0, 0.0], dtype=np.float64),
               np.array([WALHW[0], 0.0, 0.0], dtype=np.float64)]

WALL_NAMES = ["Rx_Wall", "MIF_Room_Wall", "Ceiling", "Floor", "West_Wall", "East_Wall"]

WALL_DATA = zip(WALL_NAMES, WALL_CENTERS, WALL_EDGES1, WALL_EDGES2)

WALL_DIVS = 32

PT_SOURCE_XVALS = zip(WALX, ["Front", "Midpoint", "Back"])
PT_SOURCE_YVALS = zip(WALY, ["Left", "Center", "Right"]_
PT_SOURCE_ZVALS = zip(WALZ, ["Top", "Middle", "Bottom"])


CORNER_QUARTETS = [(np.array([WALX[0], WALY[0], WALZ[0]], dtype=np.float64),
                    np.array([WALX[0], WALY[0], WALZ[2]], dtype=np.float64),
                    np.array([WALX[0], WALY[2], WALZ[0]], dtype=np.float64),
                    np.array([WALX[2], WALY[0], WALZ[0]], dtype=np.float64)),
                   (np.array([WALX[0], WALY[2], WALZ[2]], dtype=np.float64),
                    np.array([WALX[0], WALY[2], WALZ[0]], dtype=np.float64),
                    np.array([WALX[0], WALY[0], WALZ[2]], dtype=np.float64),
                    np.array([WALX[2], WALY[2], WALZ[2]], dtype=np.float64)),
                   (np.array([WALX[2], WALY[0], WALZ[2]], dtype=np.float64),
                    np.array([WALX[2], WALY[0], WALZ[0]], dtype=np.float64),
                    np.array([WALX[2], WALY[2], WALZ[2]], dtype=np.float64),
                    np.array([WALX[0], WALY[0], WALZ[2]], dtype=np.float64)),
                   (np.array([WALX[2], WALY[2], WALZ[0]], dtype=np.float64),
                    np.array([WALX[2], WALY[2], WALZ[2]], dtype=np.float64),
                    np.array([WALX[2], WALY[0], WALZ[0]], dtype=np.float64),
                    np.array([WALX[0], WALY[2], WALZ[0]], dtype=np.float64))]

QUARTET_NAMES = [("Front_Left", "Front_Top", "Left_Top"),
                 ("Front_Right", "Front_Bottom", "Right_Bottom"),
                 ("Back_Left", "Back_Bottom", "Left_Bottom"),
                 ("Back_Right", "Back_Top", "Right_Top")]

def set_up_all_sources():
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
    src_list.extend(make_point_sources())
    src_list.extend(make_cube_edge_sources())
    # src_list.extend(make_hot_patches())
    # src_list.extend(make_vertical_cylinders())
    # src_list.extend(make_beamlines())
    return src_list


def make_cube_wall_sources():
    """This function generates the sources that represent segments "walls"
    around the AD1 and position scan area

    Returns
    -------
    wall_list : list
        list of square surface objects located at "Effective" walls
    """
    wall_list = []
    wall_list.extend(make_sectioned_cube_wall_sources())
    # wall_list.extend(make_whole_cube_wall_sources())
    # wall_list.extend(make_quarter_cube_wall_sources())
    # wall_list.extend(make_ninthed_cube_wall_sources())
    # wall_list.extend(make_sixteenthed_cube_wall_sources())
    return wall_list

def make_sectioned_cube_wall_sources():
    """This function generates the sectioned wall sources where each dimension
    has the sectioning specified in WALL_DIVS

    Returns
    -------
    wall_list : list
        list of square surface objects located at "Effective" walls
    """
    wall_list = []
    # creating a rotation matrix without adding rotations makes it just the
    # identity matrix
    no_rotate = gb.Rotation()
    offset = float(WALL_DIVS-1)/float(WALL_DIVS)
    # loop through each of the walls
    for name, cent, edge1, edge2 in WALL_DATA:
        div_edge1 = edge1/float(WALL_DIVS)
        div_edge2 = edge2/float(WALL_DIVS)
        area = np.linalg.norm(div_edge1) * np.linalg.norm(div_edge2)
        # now that we have the wall data loop across the sub divisions,
        # printing the inches locations of the wall segments as we go
        for ind1 in range(WALL_DIVS):
            axis1_shift = (offset - (float(ind1)*2.0/float(WALL_DIVS)))
            for ind2 in range(WALL_DIVS):
                axis2_shift(offset - (float(ind2)*2.0/float(WALL_DIVS)))
                patch_name = name + "_{0:d}_{1:d}".format(ind1, ind2)
                patch_center = cent + axis1_shift*edge1 + axis2_shift*edge2
                print "Making patch centered at:", patch_center
                print "    It is named: ", patch_name, "It has area:", area
                wall_list.append(fs.Square(patch_name, patch_center, div_edge1,
                                           div_edge2))


def make_sixteenthed_cube_wall_sources():
    """This function generates the sixteenthed sources that represent "walls"
    around the AD1 and position scan area

    Returns
    -------
    wall_list : list
        list of square surface objects located at "Effective" walls
    """
    wall_list = []
    # creating a rotation matrix without adding rotations makes it just the
    # identity matrix
    no_rotate = gb.Rotation()
    for name, cent, edge1, edge2 in WALL_DATA:
        wall_list.append(fs.Square(name+"_16_0", cent+3.0*edge1/4.0+3.0*edge2/4.0,
                                   (edge1/4.0, edge2/4.0), no_rotate))
        wall_list.append(fs.Square(name+"_16_1", cent+3.0*edge1/4.0+edge2/4.0,
                                   (edge1/4.0, edge2/4.0), no_rotate))
        wall_list.append(fs.Square(name+"_16_2", cent+3.0*edge1/4.0-edge2/4.0,
                                   (edge1/4.0, edge2/4.0), no_rotate))
        wall_list.append(fs.Square(name+"_16_3", cent+3.0*edge1/4.0-3.0*edge2/4.0,
                                   (edge1/4.0, edge2/4.0), no_rotate))
        wall_list.append(fs.Square(name+"_16_4", cent+edge1/4.0+3.0*edge2/4.0,
                                   (edge1/4.0, edge2/4.0), no_rotate))
        wall_list.append(fs.Square(name+"_16_5", cent+edge1/4.0+edge2/4.0,
                                   (edge1/4.0, edge2/4.0), no_rotate))
        wall_list.append(fs.Square(name+"_16_6", cent+edge1/4.0-edge2/4.0,
                                   (edge1/4.0, edge2/4.0), no_rotate))
        wall_list.append(fs.Square(name+"_16_7", cent+edge1/4.0-3.0*edge2/4.0,
                                   (edge1/4.0, edge2/4.0), no_rotate))
        wall_list.append(fs.Square(name+"_16_8", cent-edge1/4.0+3.0*edge2/4.0,
                                   (edge1/4.0, edge2/4.0), no_rotate))
        wall_list.append(fs.Square(name+"_16_9", cent-edge1/4.0+edge2/4.0,
                                   (edge1/4.0, edge2/4.0), no_rotate))
        wall_list.append(fs.Square(name+"_16_10", cent-edge1/4.0-edge2/4.0,
                                   (edge1/4.0, edge2/4.0), no_rotate))
        wall_list.append(fs.Square(name+"_16_11", cent-edge1/4.0-3.0*edge2/4.0,
                                   (edge1/4.0, edge2/4.0), no_rotate))
        wall_list.append(fs.Square(name+"_16_12", cent-3.0*edge1/4.0+3.0*edge2/4.0,
                                   (edge1/4.0, edge2/4.0), no_rotate))
        wall_list.append(fs.Square(name+"_16_13", cent-3.0*edge1/4.0+edge2/4.0,
                                   (edge1/4.0, edge2/4.0), no_rotate))
        wall_list.append(fs.Square(name+"_16_14", cent-3.0*edge1/4.0-edge2/4.0,
                                   (edge1/4.0, edge2/4.0), no_rotate))
        wall_list.append(fs.Square(name+"_16_15", cent-3.0*edge1/4.0-3.0*edge2/4.0,
                                   (edge1/4.0, edge2/4.0), no_rotate))
    return wall_list


def make_ninthed_cube_wall_sources():
    """This function generates the ninthed sources that represent "walls"
    around the AD1 and position scan area

    Returns
    -------
    wall_list : list
        list of square surface objects located at "Effective" walls
    """
    wall_list = []
    # creating a rotation matrix without adding rotations makes it just the
    # identity matrix
    no_rotate = gb.Rotation()
    for name, cent, edge1, edge2 in WALL_DATA:
        wall_list.append(fs.Square(name+"_9_0", cent+2.0*edge1/3.0+2.0*edge2/3.0,
                                   (edge1/3.0, edge2/3.0), no_rotate))
        wall_list.append(fs.Square(name+"_9_1", cent+2.0*edge1/3.0,
                                   (edge1/3.0, edge2/3.0), no_rotate))
        wall_list.append(fs.Square(name+"_9_2", cent+2.0*edge1/3.0-2.0*edge2/3.0,
                                   (edge1/3.0, edge2/3.0), no_rotate))
        wall_list.append(fs.Square(name+"_9_3", cent+2.0*edge2/3.0,
                                   (edge1/3.0, edge2/3.0), no_rotate))
        wall_list.append(fs.Square(name+"_9_4", cent,
                                   (edge1/3.0, edge2/3.0), no_rotate))
        wall_list.append(fs.Square(name+"_9_5", cent-2.0*edge2/3.0,
                                   (edge1/3.0, edge2/3.0), no_rotate))
        wall_list.append(fs.Square(name+"_9_6", cent-2.0*edge1/3.0+2.0*edge2/3.0,
                                   (edge1/3.0, edge2/3.0), no_rotate))
        wall_list.append(fs.Square(name+"_9_7", cent-2.0*edge1/3.0,
                                   (edge1/3.0, edge2/3.0), no_rotate))
        wall_list.append(fs.Square(name+"_9_8", cent-2.0*edge1/3.0-2.0*edge2/3.0,
                                   (edge1/3.0, edge2/3.0), no_rotate))
    return wall_list


def make_quarter_cube_wall_sources():
    """This function generates the quarted sources that represent "walls"
    around the AD1 and position scan area

    Returns
    -------
    wall_list : list
        list of square surface objects located at "Effective" walls
    """
    wall_list = []
    # creating a rotation matrix without adding rotations makes it just the
    # identity matrix
    no_rotate = gb.Rotation()
    for name, cent, edge1, edge2 in WALL_DATA:
        wall_list.append(fs.Square(name+"_4_0", cent+edge1/2.0+edge2/2.0,
                                   (edge1/2.0, edge2/2.0), no_rotate))
        wall_list.append(fs.Square(name+"_4_1", cent+edge1/2.0-edge2/2.0,
                                   (edge1/2.0, edge2/2.0), no_rotate))
        wall_list.append(fs.Square(name+"_4_2", cent-edge1/2.0+edge2/2.0,
                                   (edge1/2.0, edge2/2.0), no_rotate))
        wall_list.append(fs.Square(name+"_4_3", cent-edge1/2.0-edge2/2.0,
                                   (edge1/2.0, edge2/2.0), no_rotate))
    return wall_list


def make_whole_cube_wall_sources():
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
    for name, cent, edge1, edge2 in WALL_DATA:
        wall_list.append(fs.Square(name, cent, (edge1, edge2), no_rotate))
    return wall_list


def make_point_sources():
    """This function generates the various point sources that we will try using

    Returns
    -------
    pt_list : list
        list of square surface objects located at "Effective" walls
    """
    pt_list = []
    for xv, xn in PT_SOURCE_XVALS:
        for yv, yn in PT_SOURCE_YVALS:
            for zv, zn in PT_SOURCE_ZVALS:
                if zn == "Middle" and yn == "Center" and xn == "Midpoint":
                    continue
                else:
                    name = "{0:s}_{1:s}_{2:s}".format(xn, yn, zn)
                    center = 2.54*np.array([xv, yv, zv], dtype=np.float64)
                    pt_list.append(lds.PointSource(name, center))
    return pt_list


def make_cube_edge_sources():
    """This function generates the sources that represent corners of the walls
    that surround the AD1 area

    Returns
    -------
    line_list : list
        list of line objects located at edges of the "Effective" walls
    """
    line_list = []
    for i, quartet in enumerate(CORNER_QUARTETS):
        pt1 = quartet[0]
        for j, pt2 in enumerate(quartet[1:]):
            line_list.append(lds.LineSource(QUARTET_NAMES[i][j], pt1, pt2))
    return line_list


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
