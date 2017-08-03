#!/usr/bin/python
"""This files contains the primary code for the position decomposition weight
calculations"""

import sys
import numpy as np
from libpd import detector as dt
from libpd import source_construction as sc
import libpd.weight_calc as wc

def main():
    """Primary entrypoint for the weight calculation code

    Parameters
    ----------
    sys.argv[1] : str
        path to the list of NaI center points
    sys.argv[2] : str
        number of cores to use for the calculation
    """
    if len(sys.argv) != 4:
        print USAGE.format(sys.argv[0])
        sys.exit()
    detectors = dt.make_nai_list(dt.read_positions(sys.argv[1]))
    num_cores = int(sys.argv[2])
    sources = sc.set_up_no_wall_sources()
    sources.extend(sc.make_cube_corner_sources())
    weights = wc.calculate_weights(detectors, sources, num_cores)
    out_file = open(sys.argv[3], 'w')
    fmt_str = "{0:d}, {1:d}, {2:s}, {3:10.8e}\n"
    for elem in weights:
        out_file.write(fmt_str.format(*elem))
    out_file.close()


USAGE = """Usage:
    {0:s} <Path To NaI Center Points File> <Number of Cores> <Ouput File Name>
"""

if __name__ == "__main__":
    main()
