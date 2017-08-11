"""This file contains the objects that allow point and line sources to be made
"""

import ctypes as ct
import numpy as np
from libpd.geom_base import Shape


class PointSource(Shape):
    """This class allows the creation of a flat patch, with some arbitrary
    center and rotation"""
    def __init__(self, name, center):
        """This creates a square, flat, patch

        Parameters
        ----------
        name : str
            The name for this source
        center : vector
            location of the point
        """
        self.name = name
        self.center = center
        self.bounds = []

    def get_num_integral_params(self):
        """Returns the number of parameters that will need to be integrated
        over

        Returns
        -------
        num_params : int
            the number of parameters that will need to be integrated over to
            integrate over the volume of the object
        """
        return 0

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
        return self.center

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
        return 1.0

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
        return lib.makePoint((self.center-offset).ctypes.data_as(ct.POINTER(ct.c_double)))

    def __str__(self):
        """Returns the string representation of the object

        Returns
        -------
        out_str : str
            The string representation of the source
        """
        out_strs = []
        out_strs.append("Point - {0:s}\n".format(self.name))
        temp = "Centered At: ({0:4.2f}, {1:4.2f}, {2:4.2f})\n"
        out_strs.append(temp.format(self.center[0], self.center[1],
                                    self.center[2]))
        return "".join(out_strs)

    __repr__ = __str__


class LineSource(Shape):
    """This class allows the creation of a flat patch, with some arbitrary
    center and rotation"""
    def __init__(self, name, start, stop):
        """This creates a square, flat, patch

        Parameters
        ----------
        name : str
            The name for this source
        start : vector
            the starting point of the line
        stop : vector
            The ending point of the line
        """
        self.name = name
        self.strt = start
        self.vec = (stop - start)
        self.len_scale = np.linalg.norm(self.vec)
        self.bounds = [(0.0, 1.0)]

    def get_num_integral_params(self):
        """Returns the number of parameters that will need to be integrated
        over

        Returns
        -------
        num_params : int
            the number of parameters that will need to be integrated over to
            integrate over the volume of the object
        """
        return 1

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
        return self.strt + args[0]*self.vec

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
        return self.len_scale

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
        return lib.makeLine((self.strt-offset).ctypes.data_as(ct.POINTER(ct.c_double)),
                            self.vec.ctypes.data_as(ct.POINTER(ct.c_double)))

    def __str__(self):
        """Returns the string representation of the object

        Returns
        -------
        out_str : str
            The string representation of the source
        """
        out_strs = []
        out_strs.append("Line - {0:s}\n".format(self.name))
        temp = "Starts At: ({0:4.2f}, {1:4.2f}, {2:4.2f})\n"
        out_strs.append(temp.format(self.strt[0], self.strt[1],
                                    self.strt[2]))
        temp = "Ends At: ({0:4.2f}, {1:4.2f}, {2:4.2f})\n"
        out_strs.append(temp.format(self.strt[0] + self.vec[0], self.strt[1] + self.vec[1],
                                    self.strt[2] + self.vec[2]))
        return "".join(out_strs)

    __repr__ = __str__
