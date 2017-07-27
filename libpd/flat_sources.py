"""This file contains the objects that allow flat sources like disks and
squares to be made"""

from libpd.geom_base import Shape
import numpy as np
import ctypes as ct


class Square(Shape):
    """This class allows the creation of a flat patch, with some arbitrary
    center and rotation"""
    def __init__(self, name, center, basis_vecs, rotation):
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
        # for this shape there is no need to store the transformation, simply
        # apply it to the two basis vectors and then ignore it
        self.vec1 = rotation.rmat.dot(basis_vecs[0])
        self.vec2 = rotation.rmat.dot(basis_vecs[1])
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

    def make_source_object(self, lib, offset):
        """This function generates a void ptr for a backend source object

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
        return lib.makeSquare((self.center-offset).ctypes.data_as(ct.POINTER(ct.c_double)),
                              self.vec1.ctypes.data_as(ct.POINTER(ct.c_double)),
                              self.vec2.ctypes.data_as(ct.POINTER(ct.c_double)))

    def __str__(self):
        """Returns the string representation of the object

        Returns
        -------
        out_str : str
            The string representation of the source
        """
        out_strs = []
        out_strs.append("Square Patch - {0:s}\n".format(self.name))
        temp = "Centered At: ({0:4.2f}, {1:4.2f}, {2:4.2f})\n"
        out_strs.append(temp.format(self.center[0], self.center[1],
                                    self.center[2]))
        temp = "First Basis Vector: ({0:4.2f}, {1:4.2f}, {2:4.2f})\n"
        out_strs.append(temp.format(self.vec1[0], self.vec1[1], self.vec1[2]))
        temp = "Second Basis Vector: ({0:4.2f}, {1:4.2f}, {2:4.2f})"
        out_strs.append(temp.format(self.vec2[0], self.vec2[1], self.vec2[2]))
        return "".join(out_strs)

    __repr__ = __str__


class Circle(Shape):
    """This class allows the creation of a flat circle, with some arbitrary
    center and rotation"""
    def __init__(self, name, center, radius, rotation):
        """This creates a flat circle, with arbitrary rotation

        Parameters
        ----------
        name : str
            The name for this source
        center : vector
            center point of the square patch
        radius : float
            radius of the circle
        rotation : geom_base.Rotation
            The rotation matrix that moves the patch into the correct
            orientation
        """
        self.name = name
        self.center = center
        # for this shape there is no need to store the transformation, simply
        # apply it to the two basis vectors and then ignore it
        self.rad = radius
        self.rmat = rotation.rmat
        self.bounds = [(0.0, 2.0*np.pi), (0.0, 1.0)]

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
            The angle of the radius vector
        scale2 : float
            The scaling of the radius vector

        Returns
        -------
        position : vector
            The position corresponding to those integration parameters
        """
        rvec = self.rmat.dot((self.rad * np.array([np.cos(args[0]),
                                                   np.sin(args[0]), 0.0])))
        return self.center + args[1]*rvec

    def make_source_object(self, lib, offset):
        """This function generates a void ptr for a backend source object

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
        return lib.makeCircle((self.center-offset).ctypes.data_as(ct.POINTER(ct.c_double)),
                              self.radius,
                              self.rmat.ctypes.data_as(ct.POINTER(ct.c_double)))

    def __str__(self):
        """Returns the string representation of the object

        Returns
        -------
        out_str : str
            The string representation of the source
        """
        out_strs = []
        out_strs.append("Circular Patch - {0:s}\n".format(self.name))
        temp = "Centered At: ({0:4.2f}, {1:4.2f}, {2:4.2f})\n"
        out_strs.append(temp.format(self.center[0], self.center[1],
                                    self.center[2]))
        out_strs.append("Radius: {0:4.2f}\n".format(self.rad))
        out_strs.append("Rotation Matrix:\n{0:s}".format(self.rmat.__str__()))
        return "".join(out_strs)

    __repr__ = __str__


class CircleXY(Shape):
    """This class allows the creation of a flat circle parallel to the XY
    plane, with some arbitrary center and *no* rotation"""
    def __init__(self, name, center, radius):
        """This creates a flat circle, with arbitrary rotation

        Parameters
        ----------
        name : str
            The name for this source
        center : vector
            center point of the square patch
        radius : float
            radius of the circle
        """
        self.name = name
        self.center = center
        # for this shape there is no need to store the transformation, simply
        # apply it to the two basis vectors and then ignore it
        self.rad = radius
        self.bounds = [(0.0, 2.0*np.pi), (0.0, 1.0)]

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
            The angle of the radius vector
        scale2 : float
            The scaling of the radius vector

        Returns
        -------
        position : vector
            The position corresponding to those integration parameters
        """
        rvec = self.rad * np.array([np.cos(args[0]), np.sin(args[0]), 0.0])
        return self.center + args[1]*rvec

    def make_source_object(self, lib, offset):
        """This function generates a void ptr for a backend source object

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
        return lib.makeCircleXY((self.center-offset).ctypes.data_as(ct.POINTER(ct.c_double)),
                                self.radius)

    def __str__(self):
        """Returns the string representation of the object

        Returns
        -------
        out_str : str
            The string representation of the source
        """
        out_strs = []
        out_strs.append("XY Circular Patch - {0:s}\n".format(self.name))
        temp = "Centered At: ({0:4.2f}, {1:4.2f}, {2:4.2f})\n"
        out_strs.append(temp.format(self.center[0], self.center[1],
                                    self.center[2]))
        out_strs.append("Radius: {0:4.2f}".format(self.rad))
        return "".join(out_strs)

    __repr__ = __str__


class CircleXZ(Shape):
    """This class allows the creation of a flat circle parallel to the XZ
    plane, with some arbitrary center and *no* rotation"""
    def __init__(self, name, center, radius):
        """This creates a flat circle, with arbitrary rotation

        Parameters
        ----------
        name : str
            The name for this source
        center : vector
            center point of the square patch
        radius : float
            radius of the circle
        """
        self.name = name
        self.center = center
        # for this shape there is no need to store the transformation, simply
        # apply it to the two basis vectors and then ignore it
        self.rad = radius
        self.bounds = [(0.0, 2.0*np.pi), (0.0, 1.0)]

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
            The angle of the radius vector
        scale2 : float
            The scaling of the radius vector

        Returns
        -------
        position : vector
            The position corresponding to those integration parameters
        """
        rvec = self.rad * np.array([np.cos(args[0]), 0.0, np.sin(args[0])])
        return self.center + args[1]*rvec

    def make_source_object(self, lib, offset):
        """This function generates a void ptr for a backend source object

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
        return lib.makeCircleXZ((self.center-offset).ctypes.data_as(ct.POINTER(ct.c_double)),
                                self.radius)

    def __str__(self):
        """Returns the string representation of the object

        Returns
        -------
        out_str : str
            The string representation of the source
        """
        out_strs = []
        out_strs.append("XZ Circular Patch - {0:s}\n".format(self.name))
        temp = "Centered At: ({0:4.2f}, {1:4.2f}, {2:4.2f})\n"
        out_strs.append(temp.format(self.center[0], self.center[1],
                                    self.center[2]))
        out_strs.append("Radius: {0:4.2f}".format(self.rad))
        return "".join(out_strs)

    __repr__ = __str__


class CircleYZ(Shape):
    """This class allows the creation of a flat circle parallel to the YZ
    plane, with some arbitrary center and *no* rotation"""
    def __init__(self, name, center, radius):
        """This creates a flat circle, with arbitrary rotation

        Parameters
        ----------
        name : str
            The name for this source
        center : vector
            center point of the square patch
        radius : float
            radius of the circle
        """
        self.name = name
        self.center = center
        # for this shape there is no need to store the transformation, simply
        # apply it to the two basis vectors and then ignore it
        self.rad = radius
        self.bounds = [(0.0, 2.0*np.pi), (0.0, 1.0)]

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
            The angle of the radius vector
        scale2 : float
            The scaling of the radius vector

        Returns
        -------
        position : vector
            The position corresponding to those integration parameters
        """
        rvec = self.rad * np.array([0.0, np.cos(args[0]), np.sin(args[0])])
        return self.center + args[1]*rvec

    def make_source_object(self, lib, offset):
        """This function generates a void ptr for a backend source object

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
        return lib.makeCircleYZ((self.center-offset).ctypes.data_as(ct.POINTER(ct.c_double)),
                                self.radius)

    def __str__(self):
        """Returns the string representation of the object

        Returns
        -------
        out_str : str
            The string representation of the source
        """
        out_strs = []
        out_strs.append("YZ Circular Patch - {0:s}\n".format(self.name))
        temp = "Centered At: ({0:4.2f}, {1:4.2f}, {2:4.2f})\n"
        out_strs.append(temp.format(self.center[0], self.center[1],
                                    self.center[2]))
        out_strs.append("Radius: {0:4.2f}".format(self.rad))
        return "".join(out_strs)

    __repr__ = __str__
