"""This file contains the class that builds the segment trees for initial use
"""

from librecseg.segment_tree import SegmentTree
from librecseg.node_list import SegmentNodeList

class TreeBuilder(object):
    names = ["Floor", "Ceiling", "East_Wall", "West_Wall", "Rx_Wall",
             "MIF_Room_Wall"]
    """This class is the object that manages the segment trees and allow
    the optimizer to easily find subdivisions and sums, etc"""
    def __init__(self, num_segments):
        """Initializes the segment manager.

        Parameters
        ----------
        num_segments : int
            The number of segments each of the two axes of the panels are
            divided into
        """
        self.nseg = num_segments
        self.trees = [SegmentTree(name, num_segments) for name in self.names]

    def get_top_panels(self):
        """Returns the roots of the 6 trees that SegmentManager created

        Returns
        -------
        panel_list : list of SegmentNodeLists
            Each element of this list is a SegmentNodeList containing just the
            top level nodes of the trees in question
        """
        return [SegmentNodeList(tree) for tree in self.trees]

    def get_num_segs(self):
        """Returns the number of segments each axis was divided into

        Returns
        -------
        num_segs : int
            The number of segments for each axis
        """
        return self.nseg
