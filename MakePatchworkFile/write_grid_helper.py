#!/usr/bin/python
import sys

def main():
    patch_data = read_patches(sys.argv[1])
    slist = [[], [], [], [], [], [], []]
    slist[0].append("{0:f}".format(patch_data[0][0][0]))
    slist[1].append("{0:f}".format(patch_data[0][0][1]))
    slist[2].append("{0:f}".format(patch_data[0][0][2]))
    slist[3].append("{0:f}".format(patch_data[0][1][0]))
    slist[4].append("{0:f}".format(patch_data[0][1][1]))
    slist[5].append("{0:f}".format(patch_data[0][1][2]))
    slist[6].append("{0:f}".format(patch_data[0][2]))
    outfile = open("rxon_grid.h", 'w')
    for center, extents, area in patch_data[1:]:
        slist[0].append(", {0:f}".format(center[0]))
        slist[1].append(", {0:f}".format(center[1]))
        slist[2].append(", {0:f}".format(center[2]))
        slist[3].append(", {0:f}".format(extents[0]))
        slist[4].append(", {0:f}".format(extents[1]))
        slist[5].append(", {0:f}".format(extents[2]))
        slist[6].append(", {0:f}".format(area))
    outfile.write(OUTSTR.format("".join(slist[0]), "".join(slist[1]),
                                "".join(slist[2]), "".join(slist[3]),
                                "".join(slist[4]), "".join(slist[5]),
                                "".join(slist[6])))
    outfile.close()


def read_patches(fname):
    infile = open(fname)
    patch_list = []
    for line in infile:
        elem = [float(x.strip()) for x in line.strip().split(',')]
        center = [elem[0], elem[2], elem[4]]
        extents = [elem[1], elem[3], elem[5]]
        area = 0.0
        if elem[1] < 0.0001:
            area = 4.0*elem[3] * elem[5]
        elif elem[3] < 0.0001:
            area = 4.0*elem[1] * elem[5]
        elif elem[5] < 0.0001:
            area = 4.0*elem[1] * elem[3]
        patch_list.append((center, extents, area))
    return patch_list

OUTSTR = """
static const double xpos[52] = {{ {0:s} }};

static const double ypos[52] = {{ {1:s} }};

static const double zpos[52] = {{ {2:s} }};

static const double xwidth[52] = {{ {3:s} }};

static const double ywidth[52] = {{ {4:s} }};

static const double zwidth[52] = {{ {5:s} }};

static const double areas[52] = {{ {6:s} }};
"""

if __name__ == "__main__":
    main()
