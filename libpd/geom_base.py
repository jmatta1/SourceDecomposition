"""This file contains the base class that is used by all geometry objects in
this library"""

import numpy as np

# the number of decimals to round the rotation matrices to, this is chosen such
# that 90.0 in cosine gives 0 (instead of 6.7e-17)
RMAT_DEC = 15

class Shape(object):
    """Base class for 1D, 2D, and 3D shapes, provides several functions to be
    overloaded in derived classes, by pythons typing system this may be
    unnecessary, but coming from a c++ background, it is what I am doing"""
    def get_num_integral_params(self):
        """Returns the number of parameters that will need to be integrated
        over

        Returns
        -------
        num_params : int
            the number of parameters that will need to be integrated over to
            integrate over the volume of the object
        """
        raise NotImplementedError("Call to unimplemented base class")

    def get_integral_bounds(self):
        """Returns the bounds of each of the parameters to be integrated over

        Returns
        -------
        bounds : list of tuples
            for each parameter to be integrated across, this list contains a
            tuple with the upper and lower bounds of the parameter
        """
        raise NotImplementedError("Call to unimplemented base class")

    def get_position(self, *args):
        """Given a set of parameter values (in the same order as the bounds
        array) this returns the x,y,z position corresponding to those bounds

        Parameters
        ----------
        *args : vector
            A list of integration parameters in the same order as the bounds
            tuple

        Returns
        -------
        position : vector
            The position corresponding to those integration parameters
        """
        raise NotImplementedError("Call to unimplemented base class")


class Rotation(object):
    """This class contains the rotation matrix for describing a shapes
    orientation"""
    def __init__(self):
        """Initializes the rotation matrix as the simple identity matrix"""
        self.rmat = np.array([[1.0, 0.0, 0.0],
                              [0.0, 1.0, 0.0],
                              [0.0, 0.0, 1.0]])

    def add_z_rot(self, theta):
        """Multiplies a rotation by theta degrees about the Z axis onto the
        rotation matrix already present

        Parameters
        ----------
        theta : float
            The angle in degrees to be rotated by
        """
        sin = np.sin(np.pi*theta/180.0)
        cos = np.cos(np.pi*theta/180.0)
        rot = np.array([[cos, -sin, 0.0],
                        [sin, cos, 0.0],
                        [0.0, 0.0, 1.0]])
        self.rmat = np.dot(rot, self.rmat)
        np.round(self.rmat, RMAT_DEC, out=self.rmat)

    def add_y_rot(self, theta):
        """Multiplies a rotation by theta degrees about the Y axis onto the
        rotation matrix already present

        Parameters
        ----------
        theta : float
            The angle in degrees to be rotated by
        """
        sin = np.sin(np.pi*theta/180.0)
        cos = np.cos(np.pi*theta/180.0)
        rot = np.array([[cos, 0.0, sin],
                        [0.0, 1.0, 0.0],
                        [-sin, 0.0, cos]])
        self.rmat = np.dot(rot, self.rmat)
        np.round(self.rmat, RMAT_DEC, out=self.rmat)

    def add_x_rot(self, theta):
        """Multiplies a rotation by theta degrees about the X axis onto the
        rotation matrix already present

        Parameters
        ----------
        theta : float
            The angle in degrees to be rotated by
        """
        sin = np.sin(np.pi*theta/180.0)
        cos = np.cos(np.pi*theta/180.0)
        rot = np.array([[1.0, 0.0, 0.0],
                        [0.0, cos, -sin],
                        [0.0, sin, cos]])
        self.rmat = np.dot(rot, self.rmat)
        np.round(self.rmat, RMAT_DEC, out=self.rmat)

    def add_unit_vec_rot(self, theta, unit_vector):
        """Multiplies a rotation by theta degrees about the given unit vector
        onto the rotation matrix already present

        Parameters
        ----------
        theta : float
            The angle in degrees to be rotated by
        unit_vector : vector
            The unit vector about which to rotate
        """
        sin = np.sin(np.pi*theta/180.0)
        cos = np.cos(np.pi*theta/180.0)
        term1 = np.array([[cos, 0.0, 0.0], [0.0, cos, 0.0], [0.0, 0.0, cos]])
        term2 = sin * np.array([[0.0, -unit_vector[2], unit_vector[1]],
                                [unit_vector[2], 0.0, -unit_vector[0]],
                                [-unit_vector[1], unit_vector[0], 0.0]])
        term3 = (1.0-cos) * np.tensordot(unit_vector, unit_vector, axes=0)
        rot = term1 + term2 + term3
        self.rmat = np.dot(rot, self.rmat)
        np.round(self.rmat, RMAT_DEC, out=self.rmat)
