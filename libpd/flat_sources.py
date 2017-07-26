"""This file contains the objects that allow flat sources like disks and
squares to be made"""

from libpd.geom_base import Shape

class Square(Shape):
    """This class allows the creation of a flat patch, with some arbitrary
    center and rotation"""
    def __init__(self, center, edge_vec1, edge_vec2, rotation):
        """This creates a square, flat, patch

        Parameters
        ----------
        center : vector
            center point of the square patch
        edge_vec1 : vector
            vector from center to the middle of one of the edges
        edge_vec2 : vector
            vector from center to the middle of another edge (must be
            orthogonal to edge_vec1)
        rotation : geom_base.Rotation
            The rotation matrix that moves the patch into the correct
            orientation
        """
        self.center = center
        # for this shape there is no need to store the transformation, simply
        # apply it to the two basis vectors and then ignore it
        self.vec1 = rotation.rmat.dot(edge_vec1)
        self.vec2 = rotation.rmat.dot(edge_vec2)
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

    def get_position(self, scale1, scale2):
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
        return self.center + scale1*self.vec1 + scale2*self.vec2
