#!/usr/bin/python
"""This file contains the code for making a 'position response matrix'
from the output of calc_pd_weights and calc_ss_weights"""

import sys
import numpy as np
import ROOT as rt

# TODO: write proper doc strings for this code
SRC_NAMES = ["Rx_Wall", "MIF_Room_Wall", "East_Wall", "West_Wall", "Ceiling",
"Floor", "EF4_Cover", "HB3_Access", "HB4_Access", "HB3", "HB3a", "HB4"]


def main(infile_name, outfile_name):
    # first open the input file
    raw_data = read_input_file(infile_name)
    # generate the numpy 2D array that will hold the data before dumping to root
    resp_mat, xsize, ysize = make_resp_mat(raw_data)
    write_resp_mat(outfile_name, resp_mat, xsize, ysize)


def write_resp_mat(outfile_name, resp_mat, xsize, ysize):
    outfile = rt.TFile(outfile_name, "RECREATE")
    out_hist = rt.TH2D("pos_resp", "Position Response Matrix", ysize, -0.5,
                       float(ysize)-0.5,xsize, -0.5, float(xsize)-0.5)
    for i in range(xsize):
        for j in range(ysize):
            binnum = out_hist.GetBin(j+1, i+1)
            out_hist.SetBinContent(binnum, resp_mat[i, j])
    out_hist.Write()
    outfile.Flush()
    outfile.Close()


def make_resp_mat(raw_data):
    # calculate the number of sources
    src_count = len(SRC_NAMES)
    # calculate how many positions there are
    pos_count = max([x[0][2] for x in raw_data]) + 1
    resp_mat = np.zeros((src_count, pos_count))
    for i, name in enumerate(SRC_NAMES):
        resp_func = [x for x in raw_data if x[1] == name]
        resp_func.sort(key=lambda x: x[0][2])
        resp_mat[i,:] = np.array([x[2] for x in resp_func])
    return (resp_mat, src_count, pos_count)


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
    {0:s} <input file name> <output root file name>
"""


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print USAGE.format(sys.argv[0])
        sys.exit()
    main(sys.argv[1], sys.argv[2])
