#!/usr/bin/python
"""This files contains the primary code for the source summation weight
calculations, i.e. it uses the infrastructure from position decomposition
to calculate the weight of each source on the AD1 'patches' """

import sys
from libpd import detector as dt
from libpd import source_construction as sc
import libpd.weight_calc as wc

def main(patch_info_path, core_count, out_name):
    """Primary entrypoint for the weight calculation code

    Parameters
    ----------
    patch_info_path : str
        path to the list of AD1 patches, normals, and sizes
    core_count : str
        number of cores to use for the calculation
    out_name : str
        name of the output file
    """
    detectors = dt.read_patches(patch_info_path)
    sources = sc.set_up_no_wall_sources()
    sources.extend(sc.make_cube_corner_sources())
    weights = wc.calculate_weights(detectors, sources, core_count)
    out_file = open(out_name, 'w')
    fmt_str = "{0:d}, {1:d}, {2:s}, {3:10.8e}\n"
    for elem in weights:
        out_file.write(fmt_str.format(*elem))
    out_file.close()


USAGE = """Usage:
    {0:s} <Path To Patch Info File> <Number of Cores> <Ouput File Name>
"""

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print USAGE.format(sys.argv[0])
        sys.exit()
    main(sys.argv[1], int(sys.argv[2]), sys.argv[3])
