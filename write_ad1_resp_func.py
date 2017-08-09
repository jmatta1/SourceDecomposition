#!/usr/bin/python
"""This file contains the code for making a 'position response matrix'
from the output of calc_pd_weights and calc_ss_weights"""

import sys
import numpy as np
import ROOT as rt

# TODO: write proper doc strings for this code
NUM_POS = 52
DET_IND_ARRAY = [[0, 1, 2, 3, 4, 5, 6, 7],
                 [8, 9,10,11,12,13,14,15],
                 [16,17,18,19,20,21,22,23,24,25,26,27],
                 [28,29,30,31,32,33,34,35,36,37,38,39],
                 [40,41,42,43,44,45],
                 [46,47,48,49,50,51]]

def main(infile_name, outfile_name):
    # first open the input file
    print "Reading Raw Data"
    raw_data = read_input_file(infile_name)
    # read the list of source names
    #print "Reading Name List"
    name_list = set([x[1] for x in raw_data])
    #with open(name_list_path) as namefile:
    #    name_list = [line.strip() for line in namefile]
    # take the response data and generate a response function for every one
    # and write that function into a root file
    print "Dumping Spectra"
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


def read_input_file(infile_name):
    infile = open(infile_name)
    out_list = []
    for line in infile:
        elems = [x.strip() for x in line.strip().split(',')]
        temp = []
        dnum = int(elems[0])
        rnum = int(elems[1])
        pnum = DET_IND_ARRAY[dnum][rnum]
        temp.append((dnum, rnum, pnum))
        temp.append(elems[2])
        temp.append(float(elems[3]))
        out_list.append(temp)
    return out_list


USAGE = """Usage:
    {0:s} <input file name> <output root file name>
"""


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print USAGE.format(sys.argv[0])
        sys.exit()
    main(sys.argv[1], sys.argv[2])
