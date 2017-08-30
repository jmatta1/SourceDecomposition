#!/usr/bin/python
import os
import sys
import subprocess as sp

CONFIG_STR = """[StartConfig]
ResetFile = {reset:s}
InputFileName = "./data/NaIResp.root"
OutputFileName = "./data/AllNaIResp.root"
FunctionSumName = "{out_name:s}"
FunctionList += "{in_name:s}"
[EndConfig]
"""

IN_NAMES = ["Floor_{0:d}_{1:d}", "Ceiling_{0:d}_{1:d}",
            "East_Wall_{0:d}_{1:d}", "West_Wall_{0:d}_{1:d}",
            "Rx_Wall_{0:d}_{1:d}", "MIF_Room_Wall_{0:d}_{1:d}"]

OUT_NAMES = ["Floor_{0:d}_{1:d}_{2:d}", "Ceiling_{0:d}_{1:d}_{2:d}",
             "East_Wall_{0:d}_{1:d}_{2:d}", "West_Wall_{0:d}_{1:d}_{2:d}",
             "Rx_Wall_{0:d}_{1:d}_{2:d}", "MIF_Room_Wall_{0:d}_{1:d}_{2:d}"]


def main(num_segs):
    doreset = True
    fmt_dict = {}
    for inname, outname in zip(IN_NAMES, OUT_NAMES):
        for i in range(num_segs):
            for j in range(num_segs):
                fmt_dict["in_name"] = inname.format(i, j)
                fmt_dict["out_name"] = outname.format(num_segs, i, j)
                fmt_dict["reset"] = "True" if doreset else "False"
                cfg_file = open("temp_cfg", 'w')
                cfg_file.write(CONFIG_STR.format(**fmt_dict))
                cfg_file.close()
                sp.call(["./funcSummer","temp_cfg"])
                doreset = False
    os.remove("temp_cfg")


if __name__ == "__main__":
    main(int(sys.argv[1]))
