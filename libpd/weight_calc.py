"""This file contains the code necessary to calculate the weights of every
source, detector pair"""


import copy as cp
import multiprocessing
import ctypes as ct
import numpy as np
from scipy import integrate as spi
# from scipy import LowLevelCallable
from libpd.detector import SimpleDetectingSurface
import libpd.backend_interface as bi

# relocated to the bottom so the functions can be found
# INT_FUNC = [integrand2d, integrand3d, integrand4d, integrand5d]
INV_FOUR_PI = (1.0/(4.0*np.pi))
NUM_BACKEND_OUT_PARAMS = 5
FMT_STR = "{0:d}, {1:d}, {2:d}, {3:s}, {4:e}, {5:d}, {6:d}, {7:d}, {8:d}"
HEADINGS = "Det, Run, Side, Source Name, Weight, Recursion Depth, Single Axis"\
    "Recursions, All Axis Recursions, Integrand Evaluations"

def calculate_weights(detectors, sources, num_cores):
    """This function calculates the weights for each source and detector
    surface pair in the detectors and sources arrays passed to it. If num_cores
    is greater than 1 it will also utilize the multiprocessing module to
    perform the calculations in parallel

    Parameters
    ----------
    detectors : list of libpd.detector.Detector classes
        List of detectors for weights to be calculated at
    sources : list of source classes derived from libpd.Shape
        List of sources for weights at each detector to be calculated for
    num_cores : int
        Number of cores to spread the computation across

    Returns
    -------
    weights_matrix : list of lists of floats
        The "Response Matrix" each sub list is the weights for a given source
        at every detector position
    """
    if num_cores == 1:
        return calculate_weights_single(detectors, sources)
    else:
        return calculate_weights_multi(detectors, sources, num_cores)


def calculate_weights_single(detectors, sources):
    """This function calculates the weights for each source and detector
    surface pair in the detectors and sources arrays passed to it in a single
    threaded fashion, using the standard map function for easy debugging

    Parameters
    ----------
    detectors : list of libpd.detector.Detector classes
        List of detectors for weights to be calculated at
    sources : list of source classes derived from libpd.Shape
        List of sources for weights at each detector to be calculated for

    Returns
    -------
    weights_matrix : list of lists of floats
        The "Response Matrix" each sub list is the weights for a given source
        at every detector position
    """
    # first generate the list of detecting surface and source pairs
    # (because for each NaI detector there are 6 surfaces, whereas for each
    # AD1 'detector' there is only one surface)
    input_list = []
    for det in detectors:
        rdat = det.get_run_data()
        for src in sources:
            for i, det_surf in enumerate(det.get_detecting_surfaces()):
                sdat = (rdat[0], rdat[1], i, src.name)
                input_list.append((sdat, cp.deepcopy(det_surf),
                                   cp.deepcopy(src)))
    print "Commencing Single Threaded Integration!"
    print "There are", len(input_list), "integrals to calculate"
    print HEADINGS
    # now calculate the weight at every position using a single core
    weight_list = [calc_weight_opt(x) for x in input_list]
    # return the sorted and summed weights
    return sort_and_sum(weight_list)


def calculate_weights_multi(detectors, sources, num_cores):
    """This function calculates the weights for each source and detector
    surface pair in the detectors and sources arrays passed to it in a single
    threaded fashion, using the standard map function for easy debugging

    Parameters
    ----------
    detectors : list of libpd.detector.Detector classes
        List of detectors for weights to be calculated at
    sources : list of source classes derived from libpd.Shape
        List of sources for weights at each detector to be calculated for
    num_cores : int
        Number of cores to spread the computation across

    Returns
    -------
    weights_matrix : list of lists of floats
        The "Response Matrix" each sub list is the weights for a given source
        at every detector position
    """
    # first generate the list of detecting surface and source pairs
    # (because for each NaI detector there are 6 surfaces, whereas for each
    # AD1 'detector' there is only one surface)
    input_list = []
    for det in detectors:
        rdat = det.get_run_data()
        for src in sources:
            for i, det_surf in enumerate(det.get_detecting_surfaces()):
                sdat = (rdat[0], rdat[1], i, src.name)
                input_list.append((sdat, cp.deepcopy(det_surf),
                                   cp.deepcopy(src)))
    # now calculate the weight at every position using multiple cores
    # first restrict the number of cores to whatever is available
    if num_cores > multiprocessing.cpu_count():
        num_cores = multiprocessing.cpu_count()
        temp = "Too many cores passed, restricting to what is available: {0:d}"
        print temp.format(num_cores)
    print "Commencing Multi Threaded Integration!"
    print "There are", len(input_list), "integrals to calculate"
    print HEADINGS
    # set up the thread pool for the multiprocessing
    mp_pool = multiprocessing.Pool(processes=num_cores)
    # process the input list with that pool
    weight_list = mp_pool.map(calc_weight_opt, input_list)
    # return the sorted and summed weights
    return sort_and_sum(weight_list)


def sort_and_sum(weight_list):
    """This function takes the calculated weight list, sorts it and then sums
    integrals that have a common position and source term

    Parameters
    ----------
    weight_list : list
        The weight list generated by mapping calc_weight_opt onto the list of
        detectors and sources

    Returns
    -------
    out_list : list
        The list of weights after summing the seperate faces of a detector into
        the total contribution of that source to that position
    """
    weight_list.sort(key=lambda x: (x[0][0], x[0][1], x[0][3], x[0][2]))
    out_list = []
    curr_weight = weight_list[0][1]
    prev_info = (weight_list[0][0][0], weight_list[0][0][1],
                 weight_list[0][0][3])
    for weight in weight_list[1:]:
        curr_info = (weight[0][0], weight[0][1], weight[0][3])
        if (prev_info[0] == curr_info[0] and prev_info[1] == curr_info[1] and
                prev_info[2] == curr_info[2]):
            curr_weight += weight[1]
        else:
            out_list.append((prev_info[0], prev_info[1], prev_info[2], curr_weight))
            prev_info = curr_info
            curr_weight = weight[1]
    out_list.append((prev_info[0], prev_info[1], prev_info[2], curr_weight))
    return out_list


def calc_weight_opt(data_tuple):
    """This function takes the data_tuple and uses it to perform the weight
    calculation for that detecting surface and source pair, the difference
    between this function and calc_weight is that this function uses the C/C++
    backend written for these purposes

    Parameters
    ----------
    data_tuple : tuple of information
        The first element is a tuple containing the det num, run number, and
        surface num the second element contains the detecting surface, the
        third element contains the source object

    Returns
    -------
    pos_info : tuple
        just a copy of the first element of the data tuple
    weight : float
        The weight calculated for that surface source pair
    """
    pos_info = data_tuple[0]
    surface = data_tuple[1]
    source = data_tuple[2]
    # make a list of the ranges, for the parameters in the surface and then
    # source
    ranges = surface.get_integral_bounds()
    if source.get_num_integral_params() != 0: #make sure the src is not a point
        ranges.extend(source.get_integral_bounds())
    else:
        # if the source params count is 0 then it is a point, perform the check
        # to see if it is in view of the surface early to avoid unnecessary
        # integrations
        test = surface.norm.dot(source.center - data_tuple[1].center)
        if not test > 0.0:
            return (pos_info, 0.0)
    # get the library and build the low-level call
    (lib, calc) = build_low_level_call(surface, source)
    # call the numerical integration
    # weight = spi.nquad(scp_call, ranges, args=(surface, source), opts=options)
    weight = np.array(range(NUM_BACKEND_OUT_PARAMS), dtype=np.float64)
    lib.calcIntegral(ct.cast(calc, ct.c_void_p),
                     weight.ctypes.data_as(ct.POINTER(ct.c_double)))
    temp = (pos_info[0], pos_info[1], pos_info[2], pos_info[3], weight[0],
            int(weight[1]), int(weight[2]), int(weight[3]), int(weight[4]))
    print FMT_STR.format(*temp)
    # free the calculator objection before returning
    lib.freeCalculator(ct.cast(calc, ct.c_void_p))
    return (pos_info, weight[0])


def build_low_level_call(surface, source):
    """This function builds the scipy lowlevel call around the backend

    Parameters
    ----------
    surface : libpd.detector.DetectingSurface
        The detector's detection surface
    source : libpd.geom_base.Shape
        The source geometry

    Return
    ------
    lib : ctypes cdll
        The initialized library
    calc : ctypes.c_void_p
        The calculator void pointer
    scp_call : scipy.LowLevelCallable
        A low level callable wrapped around the backend library interface
    """
    # first get the library interface
    lib = bi.initialize_interface()
    # then make the detector object pointer
    det = surface.make_backend_object(lib, surface.center)
    # now make the source objection
    src = source.make_backend_object(lib, surface.center)
    # now make the calculation object
    calc = lib.makeCalculator(ct.cast(det, ct.c_void_p), ct.cast(src, ct.c_void_p))
    return (lib, calc)


def calc_weight(data_tuple):
    """This function takes the data_tuple and uses it to perform the weight
    calculation for that detecting surface and source pair

    Parameters
    ----------
    data_tuple : tuple of information
        The first element is a tuple containing the det num, run number, and
        surface num the second element contains the detecting surface, the
        third element contains the source object

    Returns
    -------
    pos_info : tuple
        just a copy of the first element of the data tuple
    weight : float
        The weight calculated for that surface source pair
    """
    pos_info = data_tuple[0]
    surface = SimpleDetectingSurface(data_tuple[1])
    source = data_tuple[2]
    # pre-subtract the original surface center from the source and surface so
    # it only needs to be done once
    source.center = (source.center - data_tuple[1].center)
    # make a list of the ranges, for the parameters in the surface and then
    # source
    ranges = surface.get_integral_bounds()
    if source.get_num_integral_params() != 0: #make sure the src is not a point
        ranges.extend(source.get_integral_bounds())
    else:
        # if the source params count is 0 then it is a point, perform the check
        # to see if it is in view of the surface early to avoid unnecessary
        # integrations
        test = surface.norm.dot(source.center)
        if not test > 0.0:
            return (pos_info, 0.0)
    # if we made it here then procede
    integrand = INT_FUNC[len(ranges)-2]
    #options = {}
    # options["limit"] = 1000
    # weight = spi.nquad(integrand, ranges, args=(surface, source), opts=options)
    weight = spi.nquad(integrand, ranges, args=(surface, source))
    print pos_info, weight
    return (pos_info, weight[0])


def integrand2d(*args):
    """This function calculates the value at a given point of the integration
    in the case that the source is a point source, it does not perform the line
    of sight check because, for point sources, that is performed ahead of time

    Parameters
    ----------
    p1 : float
        first parameter of the detecting surface
    p2 : float
        second parameter of the detecting surface
    surf : libpd.geom_base.Shape
        The detector surface
    src : libpd.geom_base.Shape
        The source

    Returns
    -------
    weight : float
        1/(4*pi*((x1-x2)^2+(y1-y2)^2+(z1-z2)^2) calculated for the current
        integration parameters
    """
    surf = args[2]
    src = args[3]
    arr = surf.get_area_element(args[0], args[1])
    dist_sq = np.sum(np.square(surf.get_position(args[0], args[1]) - src.center))
    return (INV_FOUR_PI*arr)/dist_sq


def integrand3d(*args):
    """This function calculates the value at a given point of the integration
    for the case that the source is a line

    Parameters
    ----------
    p1 : float
        first parameter of the detecting surface
    p2 : float
        second parameter of the detecting surface
    p3 : float
        parameter describing distance along the line
    surf : libpd.geom_base.Shape
        The detector surface
    src : libpd.geom_base.Shape
        The source

    Returns
    -------
    weight : float
        1/(4*pi*((x1-x2)^2+(y1-y2)^2+(z1-z2)^2) calculated for the current
        integration parameters, or 0.0, if there is no line of sight
    """
    surf = args[3]
    src = args[4]
    pos = src.get_position(args[2])
    if surf.norm.dot(pos) <= 0:
        return 0.0
    arr = surf.get_area_element(args[0], args[1])
    arr *= src.get_area_element(args[2])
    dist_sq = np.sum(np.square(surf.get_position(args[0], args[1]) - pos))
    return (INV_FOUR_PI*arr)/dist_sq


def integrand4d(*args):
    """This function calculates the value at a given point of the integration
    for the case that the source is a surface

    Parameters
    ----------
    p1 : float
        first parameter of the detecting surface
    p2 : float
        second parameter of the detecting surface
    p3 : float
        first parameter of the source surface
    p4 : float
        second parameter of the source surface
    surf : libpd.geom_base.Shape
        The detector surface
    src : libpd.geom_base.Shape
        The source

    Returns
    -------
    weight : float
        1/(4*pi*((x1-x2)^2+(y1-y2)^2+(z1-z2)^2) calculated for the current
        integration parameters, or 0.0, if there is no line of sight
    """
    surf = args[4]
    src = args[5]
    pos = src.get_position(args[2], args[3])
    if surf.norm.dot(pos) <= 0:
        return 0.0
    arr = surf.get_area_element(args[0], args[1])
    arr *= src.get_area_element(args[2], args[3])
    dist_sq = np.sum(np.square(surf.get_position(args[0], args[1]) - pos))
    return (INV_FOUR_PI*arr)/dist_sq


def integrand5d(*args):
    """This function calculates the value at a given point of the integration
    for the case that the source is a volume

    Parameters
    ----------
    p1 : float
        first parameter of the detecting surface
    p2 : float
        second parameter of the detecting surface
    p3 : float
        first parameter of the source volume
    p4 : float
        second parameter of the source volume
    p5 : float
        third parameter of the source volume
    surf : libpd.geom_base.Shape
        The detector surface
    src : libpd.geom_base.Shape
        The source

    Returns
    -------
    weight : float
        1/(4*pi*((x1-x2)^2+(y1-y2)^2+(z1-z2)^2) calculated for the current
        integration parameters, or 0.0, if there is no line of sight
    """
    surf = args[5]
    src = args[6]
    pos = src.get_position(args[2], args[3], args[4])
    if surf.norm.dot(pos) <= 0:
        return 0.0
    arr = surf.get_area_element(args[0], args[1])
    arr *= src.get_area_element(args[2], args[3], args[4])
    dist_sq = np.sum(np.square(surf.get_position(args[0], args[1]) - pos))
    return (INV_FOUR_PI*arr)/dist_sq


INT_FUNC = [integrand2d, integrand3d, integrand4d, integrand5d]
