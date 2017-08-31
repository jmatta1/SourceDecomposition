#!/usr/bin/python
"""Simple script to rename the response functions from the weight computation
scheme to the recursive segmentation scheme, it also creates all the possible
2^n segmentations where n<k for 2^k=num_segs"""
import os
import sys
import math
import subprocess as sp

CONFIG_STR1 = """[StartConfig]
ResetFile = False
InputFileName = "./data/NaIResp.root"
OutputFileName = "./data/AllNaIResp.root"
FunctionSumName = "{out_name:s}"
{func_list:s}[EndConfig]
"""

CONFIG_STR2 = """[StartConfig]
ResetFile = {reset:s}
InputFileName = "./data/PanelResp.root"
OutputFileName = "./data/AllPanelResp.root"
FunctionSumName = "{out_name:s}"
{func_list:s}[EndConfig]
"""

FUNC_ADD = """FunctionList += "{0:s}"
"""

IN_NAMES = ["Ceiling_{0:d}_{1:d}", "Floor_{0:d}_{1:d}",
            "East_Wall_{0:d}_{1:d}", "West_Wall_{0:d}_{1:d}",
            "Rx_Wall_{0:d}_{1:d}", "MIF_Room_Wall_{0:d}_{1:d}"]

OUT_NAMES = ["Ceiling_{0:d}_{1:d}_{2:d}", "Floor_{0:d}_{1:d}_{2:d}",
             "East_Wall_{0:d}_{1:d}_{2:d}", "West_Wall_{0:d}_{1:d}_{2:d}",
             "Rx_Wall_{0:d}_{1:d}_{2:d}", "MIF_Room_Wall_{0:d}_{1:d}_{2:d}"]


def main(num_segs, num_divs):
    """Enumerates the full set of position response function names in the
    system used by the WeightComputation functions and then uses the funcSummer
    utility to move them to a new file with a new name in the format needed by
    the recursive segmentation code it also creates all the possible
    2^n segmentations where n<k for 2^k=num_segs"""
    doreset = True
    fmt_dict = {}
    for cfg in [CONFIG_STR1, CONFIG_STR2]:
        for inname, outname in zip(IN_NAMES, OUT_NAMES):
            # do a summing into various lesser sub_divisions
            for divs in range(num_divs-1, -1, -1):
                gen_sums(2**divs, num_segs, inname, outname, cfg)
    os.remove("temp_cfg")


def gen_sums(curr_seg, orig_seg, inname, outname, cfg):
    """This function generates the lesser segmentation response functions from
    the original segmentation response functions"""
    ratio = orig_seg / curr_seg
    fmt_dict = {}
    fmt_dict["reset"] = "False"
    for i in range(curr_seg):
        for j in range(curr_seg):
            fmt_dict["out_name"] = outname.format(curr_seg, i, j)
            fmt_dict["func_list"] = gen_funcs(i, j, ratio, inname, curr_seg)
            cfg_file = open("temp_cfg", 'w')
            cfg_file.write(cfg.format(**fmt_dict))
            cfg_file.close()
            sp.call(["./funcSummer","temp_cfg"])


def gen_funcs(ind1, ind2, ratio, inname, curr_seg):
    """This function generates the list of functions to be added together to
    make the lesser subdivided function"""
    out_list = []
    offset1 = ratio*ind1
    offset2 = ratio*ind2
    for i in range(ratio):
        for j in range(ratio):
            out_list.append(FUNC_ADD.format(inname.format(offset1 + i,
                                                          offset2 + j)))
    return "".join(out_list)


if __name__ == "__main__":
    num_segs = int(sys.argv[1])
    divs = math.log(num_segs, 2)
    if int(divs) != divs:
        print "The number of segments must be a power of two"
        sys.exit()
    main(num_segs, int(divs))
