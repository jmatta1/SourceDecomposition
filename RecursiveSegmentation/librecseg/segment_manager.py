"""This file contains the classes that manage segments and their sub segments
"""

from librecseg.segment_tree import SegmentTree

WALL_SEGMENT_NAMES = ["Floor", "Ceiling", "East_Wall",
                      "West_Wall", "Rx_Wall", "MIF_Room_Wall"]


class SegmentManager(object):
    """This class is the object that manages the determining segment adjacency
    for the summing of segments and running the function summing code to
    generate new functions"""
    def __init__(self, num_segments):
        self.nseg = num_segments
        # segments that are part of the standard splitting each segment in half
        # on both axes, these are stored as a tree structure for each wall
        self.std_segs = []

