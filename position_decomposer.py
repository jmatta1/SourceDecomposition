#!/usr/bin/python
"""This files contains the primary code for the position decomposition weight
calculations"""

import sys
from libpd import detector as dt

def main():
    """Primary entrypoint for the weight calculation code

    Parameters
    ----------
    sys.argv[1] : str
        path to the list of NaI center points
    sys.argv[2] : str
        number of cores to use for the calculation
    """
    if len(sys.argv) != 3:
        print USAGE.format(sys.argv[0])
        sys.exit()
    detectors = dt.make_nai_list(dt.read_positions(sys.argv[1]))
    num_cores = int(sys.argv[2])


USAGE = """Usage:
    {0:s} <Path To NaI Center Points File> <Number of Cores>
"""

if __name__ == "__main__":
    main()
