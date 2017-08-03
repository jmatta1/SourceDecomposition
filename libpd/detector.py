"""This file contains the classes necessary to build the detecting surfaces of
a detector, these can be the 6 outer surfaces of the NaI detector or the single
surface of one of the patches on the outer surface of AD1"""

import ctypes as ct
import numpy as np
from libpd.geom_base import Shape

SURF_OFFSETS = [2.54*np.array([1.125, 0.0, 0.0], dtype=np.float64),   # front
                2.54*np.array([-1.125, 0.0, 0.0], dtype=np.float64),  # back
                2.54*np.array([0.0, 8.125, 0.0], dtype=np.float64),   # left
                2.54*np.array([0.0, -8.125, 0.0], dtype=np.float64),  # right
                2.54*np.array([0.0, 0.0, 2.125], dtype=np.float64),   # top
                2.54*np.array([0.0, 0.0, -2.125], dtype=np.float64)]  # bottom

SURF_VECTORS = [(2.54*np.array([0.0, 2.125, 0.0], dtype=np.float64),  # front height
                 2.54*np.array([0.0, 0.0, 8.125], dtype=np.float64)), # front width
                (2.54*np.array([0.0, 2.125, 0.0], dtype=np.float64),  # back height
                 2.54*np.array([0.0, 0.0, 8.125], dtype=np.float64)), # back width
                (2.54*np.array([0.0, 2.125, 0.0], dtype=np.float64),  # left height
                 2.54*np.array([1.125, 0.0, 0.0], dtype=np.float64)), # left length
                (2.54*np.array([0.0, 2.125, 0.0], dtype=np.float64),  # right height
                 2.54*np.array([1.125, 0.0, 0.0], dtype=np.float64)), # right length
                (2.54*np.array([1.125, 0.0, 0.0], dtype=np.float64),  # top length
                 2.54*np.array([0.0, 0.0, 8.125], dtype=np.float64)), # top width
                (2.54*np.array([1.125, 0.0, 0.0], dtype=np.float64),  # bottom length
                 2.54*np.array([0.0, 0.0, 8.125], dtype=np.float64))] # bottom width

SURF_NORMALS = [np.array([1.0, 0.0, 0.0], dtype=np.float64),          # front normal
                np.array([-1.0, 0.0, 0.0], dtype=np.float64),         # back normal
                np.array([0.0, 1.0, 0.0], dtype=np.float64),          # left normal
                np.array([0.0, -1.0, 0.0], dtype=np.float64),         # right normal
                np.array([0.0, 0.0, 1.0], dtype=np.float64),          # top normal
                np.array([0.0, 0.0, -1.0], dtype=np.float64)]         # bottom normal


class SimpleDetectingSurface(Shape):
    """This class is used as the surface of a detector which we need to
    integrate across to calculate the contributions of a source to the flux
    incident upon the 'visible' side of the surface, unlike sources, these
    patches are always perpendicular to one of the principle axes, no
    rotations, this variant is offset to center at zero for simplified
    position calculations"""
    def __init__(self, rhs):
        """Initializes a detecting surface object

        Parameters
        ----------
        rhs : DetectingSurface
            The original detecting surface that this is based on
        """
        self.vec1 = rhs.vec1
        self.vec2 = rhs.vec2
        self.area_scale = np.linalg.norm(self.vec1) * np.linalg.norm(self.vec2)
        self.norm = rhs.norm
        self.bounds = rhs.bounds

    def get_num_integral_params(self):
        """Returns the number of parameters that will need to be integrated
        over

        Returns
        -------
        num_params : int
            the number of parameters that will need to be integrated over to
            integrate over the volume of the object
        """
        return len(self.bounds)

    def get_integral_bounds(self):
        """Returns the bounds of each of the parameters to be integrated over

        Returns
        -------
        bounds : list of lists of tuples
            for each parameter to be integrated across, each sublist contains a
            tuple with the upper and lower bounds of the parameter, each sub-
            list is one set of bounds
        """
        return self.bounds

    def get_position(self, *args):
        """Given a set of parameter values (in the same order as the bounds
        array) this returns the x,y,z position corresponding to those bounds

        Parameters
        ----------
        scale1 : float
            The v1 scaling factor
        scale2 : float
            The v2 scaling factor

        Returns
        -------
        position : vector
            The position corresponding to those integration parameters
        """
        return args[0]*self.vec1 + args[1]*self.vec2

    def get_area_element(self, *args):
        """Given a set of parameter values (in the same order as the bounds
        array) this returns the area scaling factor corresponding to those
        parameters

        Parameters
        ----------
        *args : vector
            A list of integration parameters in the same order as the bounds
            tuple

        Returns
        -------
        area_scale : float
            The area scaling factor
        """
        return self.area_scale

    def make_backend_object(self, lib, offset):
        """This function generates a void ptr for the right backend object

        Parameters
        ----------
        lib : ctypes.cdll
            The link to the backend library
        offset : numpy vector
            unused

        Returns
        -------
        src_obj : ctypes.c_void_p
            The pointer to the source object
        """
        return lib.makeDetector(self.vec1.ctypes.data_as(ct.POINTER(ct.c_double)),
                                self.vec2.ctypes.data_as(ct.POINTER(ct.c_double)),
                                self.norm.ctypes.data_as(ct.POINTER(ct.c_double)))


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
        normal : vector
            the direction of the outward face of the detector
        """
        self.center = center
        self.vec1 = edge_vec1
        self.vec2 = edge_vec2
        self.area_scale = np.linalg.norm(self.vec1) * np.linalg.norm(self.vec2)
        self.norm = normal
        self.bounds = [(-1.0, 1.0), (-1.0, 1.0)]

    def get_num_integral_params(self):
        """Returns the number of parameters that will need to be integrated
        over

        Returns
        -------
        num_params : int
            the number of parameters that will need to be integrated over to
            integrate over the volume of the object
        """
        return len(self.bounds)

    def get_integral_bounds(self):
        """Returns the bounds of each of the parameters to be integrated over

        Returns
        -------
        bounds : list of lists of tuples
            for each parameter to be integrated across, each sublist contains a
            tuple with the upper and lower bounds of the parameter, each sub-
            list is one set of bounds
        """
        return self.bounds

    def get_position(self, *args):
        """Given a set of parameter values (in the same order as the bounds
        array) this returns the x,y,z position corresponding to those bounds

        Parameters
        ----------
        scale1 : float
            The v1 scaling factor
        scale2 : float
            The v2 scaling factor

        Returns
        -------
        position : vector
            The position corresponding to those integration parameters
        """
        return self.center + args[0]*self.vec1 + args[1]*self.vec2

    def get_area_element(self, *args):
        """Given a set of parameter values (in the same order as the bounds
        array) this returns the area scaling factor corresponding to those
        parameters

        Parameters
        ----------
        *args : vector
            A list of integration parameters in the same order as the bounds
            tuple

        Returns
        -------
        area_scale : float
            The area scaling factor
        """
        return self.area_scale

    def make_backend_object(self, lib, offset):
        """This function generates a void ptr for the right backend object

        Parameters
        ----------
        lib : ctypes.cdll
            The link to the backend library
        offset : numpy vector
            The center of the detector surface object to be subtracted from
            this sources position

        Returns
        -------
        src_obj : ctypes.c_void_p
            The pointer to the source object
        """
        return lib.makeDetector(self.vec1.ctypes.data_as(ct.POINTER(ct.c_double)),
                                self.vec2.ctypes.data_as(ct.POINTER(ct.c_double)),
                                self.norm.ctypes.data_as(ct.POINTER(ct.c_double)))

    def __str__(self):
        """Returns the string representation of the object

        Returns
        -------
        out_str : str
            The string representation of the source
        """
        out_strs = []
        out_strs.append("  Detecting Surface")
        temp = "    Centered At: ({0:4.2f}, {1:4.2f}, {2:4.2f})\n"
        out_strs.append(temp.format(self.center[0], self.center[1],
                                    self.center[2]))
        temp = "    First Basis Vector: ({0:4.2f}, {1:4.2f}, {2:4.2f})\n"
        out_strs.append(temp.format(self.vec1[0], self.vec1[1], self.vec1[2]))
        temp = "    Second Basis Vector: ({0:4.2f}, {1:4.2f}, {2:4.2f})"
        out_strs.append(temp.format(self.vec2[0], self.vec2[1], self.vec2[2]))
        return "".join(out_strs)

    __repr__ = __str__


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
            self.surf.append(DetectingSurface(cent+center, vec[0], vec[1],
                                              norm))

    def get_run_data(self):
        """Returns the detector number and run number as a pair

        Returns
        -------
        det_num : int
            The detector number at this position
        run_num : int
            The run number for this position
        """
        return (self.dnum, self.rnum)

    def get_detecting_surfaces(self):
        """Returns the list of detecting surfaces for integration and for
        visibility testing

        Returns
        -------
        self.surf : list of DetectingSurface
            The list of the six surfaces that each Detector object has
        """
        return self.surf

    def __str__(self):
        """Returns the string representation of the object

        Returns
        -------
        out_str : str
            The string representation of the source
        """
        out_strs = []
        out_strs.append("Detector {0:d} Run {1:d}".format(self.dnum,
                                                          self.rnum))
        for surf in self.surf:
            out_strs.append(surf.__str__())
        return "".join(out_strs)

    __repr__ = __str__


class ProspectPatch(object):
    """This class represents one of the patches on the side of the prospect
    detector
    """
    def __init__(self, run_data, center, norm, vec):
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
        self.dnum = run_data[0]
        self.rnum = run_data[1]
        self.cent = center
        self.surf = []
        self.surf.append(DetectingSurface(center, vec[0], vec[1], norm))
            

    def get_run_data(self):
        """Returns the detector number and run number as a pair

        Returns
        -------
        det_num : int
            The detector number at this position
        run_num : int
            The run number for this position
        """
        return (self.dnum, self.rnum)

    def get_detecting_surfaces(self):
        """Returns the list of detecting surfaces for integration and for
        visibility testing

        Returns
        -------
        self.surf : list of DetectingSurface
            The list of the six surfaces that each Detector object has
        """
        return self.surf

    def __str__(self):
        """Returns the string representation of the object

        Returns
        -------
        out_str : str
            The string representation of the source
        """
        out_strs = []
        out_strs.append("Patch {0:d}, {1:d}".format(self.dnum, self.rnum))
        for surf in self.surf:
            out_strs.append(surf.__str__())
        return "".join(out_strs)

    __repr__ = __str__


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

def read_patches(fname):
    """Reads a file of AD1 "detecting patches"

    Parameters
    ----------
    fname : str
        The path to the file of detecting patches on AD1

    Returns
    -------
    patch_list : list of ProspectPatch
        The list of detecting patches on prospect
    """
    infile = open(fname)
    patch_list = []
    for line in infile:
        if line[0] == '#' or len(line) == 0:  # ignore comments and blanks
            continue
        elem = [x.strip() for x in line.strip().split(',')]
        rdat = (int(elem[0]), int(elem[1]))
        center = np.array([float(elem[2]), float(elem[3]), float(elem[4])],
                           dtype=np.float64)
        norm = np.array([float(elem[5]), float(elem[6]), float(elem[7])],
                        dtype=np.float64)
        extent_x = float(elem[8])
        extent_y = float(elem[9])
        extent_z = float(elem[10])
        vec1 = None
        vec2 = None
        if extent_x > 0.0001:
            if vec1 is None:
                vec1 = np.array([extent_x, 0.0, 0.0], dtype=np.float64)
            elif vec2 is None:
                vec2 = np.array([extent_x, 0.0, 0.0], dtype=np.float64)
        if extent_y > 0.0001:
            if vec1 is None:
                vec1 = np.array([0.0, extent_y, 0.0], dtype=np.float64)
            elif vec2 is None:
                vec2 = np.array([0.0, extent_y, 0.0], dtype=np.float64)
        if extent_z > 0.0001:
            if vec1 is None:
                vec1 = np.array([0.0, 0.0, extent_z], dtype=np.float64)
            elif vec2 is None:
                vec2 = np.array([0.0, 0.0, extent_z], dtype=np.float64)
        patch_list.append(ProspectPatch(rdat, center, norm, (vec1, vec2)))
    return patch_list


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
            detn = (i + 8 if i < 2 else i + 9)
            center = 2.54 * np.array([float(elems[1+3*i]), float(elems[2+3*i]),
                                      float(elems[3+3*i])], dtype=np.float64)
            pos_list.append((detn, runn, center))
    return pos_list
