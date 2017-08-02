"""This file contains the function that does the setup for the ctypes
interface to the shared object"""

import ctypes as ct
import os

MOD_DIR = os.path.dirname(os.path.abspath(__file__))
LIB_LOCATION = os.path.join(MOD_DIR, "libPositionDecomp.so")


def initialize_interface():
    """This function creates the shared object link and sets the appropriate
    argtypes and restypes for all the functions therein. It then returns the
    shared object link

    Returns
    -------
    lib : ctypes.cdll object
        The object for the loaded backend library
    """
    lib = ct.cdll.LoadLibrary(LIB_LOCATION)
    # set up the detectior building function
    lib.makeDetector.restype = ct.c_void_p
    lib.makeDetector.argtypes = [ct.POINTER(ct.c_double),
                                 ct.POINTER(ct.c_double),
                                 ct.POINTER(ct.c_double)]
    # set up the calculator building and freeing functions
    lib.makeCalculator.restype = ct.c_void_p
    lib.makeCalculator.argtypes = [ct.c_void_p, ct.c_void_p]
    lib.freeCalculator.argtypes = [ct.c_void_p]
    # set up the function that does the calculation of the full integral
    lib.calcIntegral.restype = ct.POINTER(ct.c_double)
    lib.calcIntegral.argtypes = [ct.c_void_p, ct.POINTER(ct.c_double)]
    # now call the functions that add all the shape creation functions
    initialize_flat_shape_funcs(lib)
    initialize_shell_shape_funcs(lib)
    return lib


def initialize_shell_shape_funcs(lib):
    """This function initializes the argtypes and restypes for the make shell
    shape functions within lib

    Parameters
    ----------
    lib : ctypes.cdd object
        The object for the loaded backend library
    """
    # the vertical cylinder
    lib.makeVertCylinder.restype = ct.c_void_p
    lib.makeVertCylinder.argtypes = [ct.POINTER(ct.c_double), ct.c_double,
                                     ct.c_double]
    # the rotated horizontal cylinder
    lib.makeRotXaxisCylinder.restype = ct.c_void_p
    lib.makeRotXaxisCylinder.argtypes = [ct.POINTER(ct.c_double), ct.c_double,
                                         ct.c_double, ct.c_double]


def initialize_flat_shape_funcs(lib):
    """This function initializes the argtypes and restypes for the make flat
    shape functions within lib

    Parameters
    ----------
    lib : ctypes.cdd object
        The object for the loaded backend library
    """
    # do the make square function
    lib.makeSquare.restype = ct.c_void_p
    lib.makeSquare.argtypes = [ct.POINTER(ct.c_double),
                               ct.POINTER(ct.c_double),
                               ct.POINTER(ct.c_double)]
    # setup the general make circle function
    lib.makeCircle.restype = ct.c_void_p
    lib.makeCircle.argtypes = [ct.POINTER(ct.c_double), ct.c_double,
                               ct.POINTER(ct.c_double)]
    # setup the make planar circle functions
    lib.makeCircleXY.restype = ct.c_void_p
    lib.makeCircleXY.argtypes = [ct.POINTER(ct.c_double), ct.c_double]
    lib.makeCircleXZ.restype = ct.c_void_p
    lib.makeCircleXZ.argtypes = [ct.POINTER(ct.c_double), ct.c_double]
    lib.makeCircleYZ.restype = ct.c_void_p
    lib.makeCircleYZ.argtypes = [ct.POINTER(ct.c_double), ct.c_double]
