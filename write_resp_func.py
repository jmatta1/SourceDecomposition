#!/usr/bin/python
"""This file contains the code for making a 'position response matrix'
from the output of calc_pd_weights and calc_ss_weights"""

import sys
import numpy as np
import ROOT as rt

# TODO: write proper doc strings for this code
NUM_POS = 109

def main(name_list_path, infile_name, outfile_name):
    # first open the input file
    print "Reading Raw Data"
    raw_data = read_input_file(infile_name)
    # read the list of source names
    print "Reading Name List"
    name_list = None
    with open(name_list_path) as namefile:
        name_list = [line.strip() for line in namefile]
    # take the response data and generate a response function for every one
    # and write that function into a root file
    write_resp_func(outfile_name, raw_data, name_list)


def write_resp_func(outfile_name, raw_data, name_list):
    outfile = rt.TFile(outfile_name, "RECREATE")
    for j, name in enumerate(name_list):
        print "Dumping position response function for",name
        out_hist = rt.TH1D(name, name+" Response Function", NUM_POS, -0.5,
                           float(NUM_POS)-0.5)
        data = [x for x in raw_data if x[1] == name]
        data.sort(key=lambda x: x[0][2])
        for i, dat in enumerate(data):
            out_hist.SetBinContent(i+1, dat[2])
        out_hist.Write()
        outfile.Flush()
        out_hist.Delete()
    outfile.Close()


def print_index_arrays(raw_data):
    base_fmt_str = "{{ {0:02d}"
    fmt_str = ", {0:02d}"
    data = [x for x in raw_data if x[1] == SRC_NAMES[0]]
    det_str = base_fmt_str.format(data[0][0][0])
    run_str = base_fmt_str.format(data[0][0][1])
    for x in data[1:]:
        det_str += fmt_str.format(x[0][0])
        run_str += fmt_str.format(x[0][1])
    outfile = open("lookup_arrays", 'w')
    outfile.write(det_str)
    outfile.write("}\n\n")
    outfile.write(run_str)
    outfile.write("}\n")


def read_input_file(infile_name):
    infile = open(infile_name)
    out_list = []
    for line in infile:
        elems = [x.strip() for x in line.strip().split(',')]
        temp = []
        dnum = int(elems[0])
        rnum = int(elems[1])
        pnum = (dnum - (8 if dnum <10 else 9))*15 + rnum
        temp.append((dnum, rnum, pnum))
        temp.append(elems[2])
        temp.append(float(elems[3]))
        out_list.append(temp)
    return out_list


USAGE = """Usage:
    {0:s} <name_list_path> <input file name> <output root file name>
"""


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print USAGE.format(sys.argv[0])
        sys.exit()
    main(sys.argv[1], sys.argv[2], sys.argv[3])
