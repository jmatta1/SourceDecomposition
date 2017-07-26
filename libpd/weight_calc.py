"""This file contains the code necessary to calculate the weights of every
source, detector pair"""


import copy as cp
import multiprocessing
import numpy as np
from scipy import integrate as spi
from libpd.detector import SimpleDetectingSurface

# relocated to the bottom so the functions can be found
# INT_FUNC = [integrand2d, integrand3d, integrand4d, integrand5d]
INV_FOUR_PI = (1.0/(4.0*np.pi))


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
        for i, det_surf in enumerate(det.get_detecting_surfaces()):
            for src in sources:
                sdat = (rdat[0], rdat[1], i, src.name)
                input_list.append((sdat, cp.deepcopy(det_surf),
                                   cp.deepcopy(src)))
    print "Commencing Integration!"
    # now calculate the weight at every position using a single core
    weight_list = [calc_weight_opt(x) for x in input_list]
    # now return the summed weights
    return weight_list


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
        for i, det_surf in enumerate(det.get_detecting_surfaces()):
            for src in sources:
                sdat = (rdat[0], rdat[1], i, src.name)
                input_list.append((sdat, cp.deepcopy(det_surf),
                                   cp.deepcopy(src)))
    # now calculate the weight at every position using multiple cores
    # first restrict the number of cores to whatever is available
    if num_cores > multiprocessing.cpu_count():
        num_cores = multiprocessing.cpu_count()
        temp = "Too many cores passed, restricting to what is available: {0:d}"
        print temp.format(num_cores)
    print "Commencing Integration!"
    # set up the thread pool for the multiprocessing
    mp_pool = multiprocessing.Pool(processes=num_cores)
    # process the input list with that pool
    weight_list = mp_pool.map(calc_weight_opt, input_list)
    # now return the summed weights
    return weight_list


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
    pass


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
    options = {}
    options["limit"] = 1000
    weight = spi.nquad(integrand, ranges, args=(surface, source), opts=options)
    print pos_info, weight
    return (pos_info, weight)


def integrand2d(p1, p2, surf, src):
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
    dist_sq = np.sum(np.square(surf.get_position(p1, p2) - src.center))
    return (INV_FOUR_PI/dist_sq)


def integrand3d(p1, p2, p3, surf, src):
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
    pos = src.get_position(p3)
    if surf.norm.dot(pos) <= 0:
        return 0.0
    dist_sq = np.sum(np.square(surf.get_position(p1, p2) - pos))
    return (INV_FOUR_PI/dist_sq)


def integrand4d(p1, p2, p3, p4, surf, src):
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
    pos = src.get_position(p3, p4)
    if surf.norm.dot(pos) <= 0:
        return 0.0
    dist_sq = np.sum(np.square(surf.get_position(p1, p2) - pos))
    return (INV_FOUR_PI/dist_sq)


def integrand5d(p1, p2, p3, p4, p5, surf, src):
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
    pos = src.get_position(p3, p4, p5)
    if surf.norm.dot(pos) <= 0:
        return 0.0
    dist_sq = np.sum(np.square(surf.get_position(p1, p2) - pos))
    return (INV_FOUR_PI/dist_sq)


INT_FUNC = [integrand2d, integrand3d, integrand4d, integrand5d]
