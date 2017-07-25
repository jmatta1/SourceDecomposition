"""This file contains the classes necessary to build the detecting surfaces of
a detector, these can be the 6 outer surfaces of the NaI detector or the single
surface of one of the patches on the outer surface of AD1"""

import numpy as np
from libpd.geom_base import Shape

SURF_OFFSETS = [2.54*np.array([1.125, 0.0, 0.0]), 2.54*np.array([-1.125, 0.0, 0.0]),
                2.54*np.array([0.0, 2.125, 0.0]), 2.54*np.array([0.0, -2.125, 0.0]),
                2.54*np.array([0.0, 0.0, 8.125]), 2.54*np.array([0.0, 0.0, -8.125])]

SURF_VECTORS = [(2.54*np.array([0.0, 2.125, 0.0]), 2.54*np.array([0.0, 0.0, 8.125])),
                (2.54*np.array([0.0, 2.125, 0.0]), 2.54*np.array([0.0, 0.0, 8.125])),
                (2.54*np.array([0.0, 2.125, 0.0]), 2.54*np.array([1.125, 0.0, 0.0])),
                (2.54*np.array([0.0, 2.125, 0.0]), 2.54*np.array([1.125, 0.0, 0.0])),
                (2.54*np.array([1.125, 0.0, 0.0]), 2.54*np.array([0.0, 0.0, 8.125])),
                (2.54*np.array([1.125, 0.0, 0.0]), 2.54*np.array([0.0, 0.0, 8.125]))]

SURF_NORMALS = [np.array([1.0, 0.0, 0.0]), np.array([-1.0, 0.0, 0.0]),
                np.array([0.0, 1.0, 0.0]), np.array([0.0, -1.0, 0.0]),
                np.array([0.0, 0.0, 1.0]), np.array([0.0, 0.0, -1.0])]

class DetectingSurface(Shape):
    """This class is used as the surface of a detector which we need to
    integrate across to calculate the contributions of a source to the flux
    incident upon the 'visible' side of the surface, unlike sources, these
    patches are always perpendicular to one of the principle axes, no
    rotations"""
    def __init__(self, center, edge_vec1, edge_vec2, normal):
        """Initializes a detecting surface object

        Parameters
        ----------
        center : vector
            center point of the square patch
        edge_vec1 : vector
            vector from center to the middle of one of the edges
        edge_vec2 : vector
            vector from center to the middle of another edge (must be
            orthogonal to edge_vec1)
        """
        self.center = center
        self.vec1 = edge_vec1
        self.vec2 = edge_vec2
        self.norm = normal

    def get_num_integral_params(self):
        return 2

    def get_integral_bounds(self):
        return [(-1.0, 1.0), (-1.0, 1.0)]

    def get_position(self, *args):
        return (self.center + args[0]*self.vec1 + args[1]*self.vec2)


class Detector(object):
    """This class represents one of the NaI detectors, holding the center,
    the run number, the detector number and the six detecting surfaces of the
    detector"""
    def __init__(self, det_num, run_num, center):
        """Initializes a Detector object from a det number, run number, and a
        position vector to the detector center

        Parameters
        ----------
        det_num : int
            the index of the detector
        run_num : int
            the run number for the position scan
        center : vector
            the position vector of the center of the detector
        """
        self.dnum = det_num
        self.rnum = run_num
        self.cent = center
        self.surf = []
        for cent, vec, norm in zip(SURF_OFFSETS, SURF_VECTORS, SURF_NORMALS):
            self.surf.append(DetectingSurface(cent, vec[0], vec[1], norm))

    def get_detecting_surfaces(self):
        """Returns the list of detecting surfaces for integration and for
        visibility testing

        Returns
        -------
        self.surf : list of DetectingSurface
            The list of the six surfaces that each Detector object has
        """
        return self.surf


def make_nai_list(centers_list):
    """Convenience function to take a list or det_nums, run_nums, and centers
    and generate a detector for each entry in that list

    Parameters:
    centers_list : list
        Each element of the list is a tuple containing the det_num, run_num,
        and center position vector for that run

    Returns:
    detector_list : list
        List of Detector objects
    """
    return [Detector(*x) for x in centers_list]


def read_positions(fname):
    """Reads a file of NaI center points to calculate detector faces for

    Parameters
    ----------
    fname : str
        The path to the file of detector centers

    Returns
    -------
    pos_list : list of tuples
        Each tuple in the list contains the detector number, scan run number,
        and x,y,z center position of the relevant NaI detector
    """
    infile = open(fname)
    pos_list = []
    for line in infile:
        elems = [val.strip() for val in line.strip().split(',')]
        runn = int(elems[0])
        for i in range(7):
            if i==2:
                continue
            detn = (i+8 if i<2 else i+9)
            center = np.array([float(elems[1+3*i]), float(elems[2+3*i]),
                               float(elems[3+3*i])])
            pos_list.append((detn, runn, center))
    return pos_list
