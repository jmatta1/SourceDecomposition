"""This file contains the objects that allow shells of 3d objects to be made,
like cylinders"""

import ctypes as ct
import numpy as np
from libpd.geom_base import Shape

class VertCylinder(Shape):
    """This class allows the creation of a flat patch, with some arbitrary
    center and rotation"""
    def __init__(self, name, center, extent):
        """This creates a square, flat, patch

        Parameters
        ----------
        name : str
            The name for this source
        center : vector
            center point of the square patch
        basis_vecs : tuple of vectors
            tuple of two orthogonal vectors from the center to the middle of
            edges
        rotation : geom_base.Rotation
            The rotation matrix that moves the patch into the correct
            orientation
        """
        self.name = name
        self.center = center
        self.rad = extent[0]
        self.len = extent[1]/2.0
        self.zvec = np.array([0.0, 0.0, extent[1]/2.0], dtype=np.float64)
        self.bounds = [(0.0, 2.0*np.pi), (-1.0, 1.0)]

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
        theta : float
            The angle of the vector
        scale2 : float
            the z-axis scaling factor

        Returns
        -------
        position : vector
            The position corresponding to those integration parameters
        """
        rvec = self.rad * np.array([np.cos(args[0]), np.sin(args[0]), 0.0],
                                   dtype=np.float64)
        return self.center + rvec + args[1]*self.zvec

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
        return self.rad*self.len

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
        return lib.makeVertCylinder((self.center-offset).ctypes.data_as(ct.POINTER(ct.c_double)),
                                    self.rad, self.len)

    def __str__(self):
        """Returns the string representation of the object

        Returns
        -------
        out_str : str
            The string representation of the source
        """
        out_strs = []
        out_strs.append("Vertical Cylindrical Shell - {0:s}\n".format(self.name))
        temp = "Centered At: ({0:4.2f}, {1:4.2f}, {2:4.2f})\n"
        out_strs.append(temp.format(self.center[0], self.center[1],
                                    self.center[2]))
        out_strs.append("Radius: {0:4.2f}\n".format(self.rad))
        out_strs.append("Length: {0:4.2f}\n".format(self.len))
        return "".join(out_strs)

    __repr__ = __str__


class RotXaxisCylinder(Shape):
    """This class allows the creation of a flat patch, with some arbitrary
    center and rotation"""
    def __init__(self, name, center, extent, theta):
        """This creates a square, flat, patch

        Parameters
        ----------
        name : str
            The name for this source
        center : vector
            center point of the square patch
        basis_vecs : tuple of vectors
            tuple of two orthogonal vectors from the center to the middle of
            edges
        rotation : geom_base.Rotation
            The rotation matrix that moves the patch into the correct
            orientation
        """
        self.name = name
        self.center = center
        self.sizes = (extent[0], extent[1]/2.0)
        self.angle = theta
        sin = np.sin(np.pi*theta/180.0)
        cos = np.cos(np.pi*theta/180.0)
        self.rmat = np.array([[cos, -sin, 0.0], [sin, cos, 0.0],
                              [0.0, 0.0, 1.0]], dtype=np.float64)
        self.xvec = np.array([extent[1]/2.0, 0.0, 0.0], dtype=np.float64)
        self.bounds = [(0.0, 2.0*np.pi), (-1.0, 1.0)]

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
        theta : float
            The angle of the vector
        scale2 : float
            the z-axis scaling factor

        Returns
        -------
        position : vector
            The position corresponding to those integration parameters
        """
        rvec = self.sizes[0] * np.array([np.cos(args[0]), 0.0, np.sin(args[0])],
                                        dtype=np.float64)
        return self.center + self.rmat.dot(rvec) + args[1]*self.xvec

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
        return self.sizes[0]*self.sizes[1]

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
        temp = (self.center-offset)
        return lib.makeRotXaxisCylinder(temp.ctypes.data_as(ct.POINTER(ct.c_double)),
                                        self.sizes[0], self.sizes[1], self.angle)

    def __str__(self):
        """Returns the string representation of the object

        Returns
        -------
        out_str : str
            The string representation of the source
        """
        out_strs = []
        out_strs.append("Cylindrical Shell - {0:s}\n".format(self.name))
        temp = "Centered At: ({0:4.2f}, {1:4.2f}, {2:4.2f})\n"
        out_strs.append(temp.format(self.center[0], self.center[1],
                                    self.center[2]))
        out_strs.append("Radius: {0:4.2f}\n".format(self.sizes[0]))
        out_strs.append("Length: {0:4.2f}\n".format(self.sizes[1]))
        out_strs.append("Rotation Matrix:\n{0:s}".format(self.rmat.__str__()))
        return "".join(out_strs)

    __repr__ = __str__
