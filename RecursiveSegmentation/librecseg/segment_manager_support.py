"""This file contains classes needed by the segment manager"""

class SegmentNodeList(object):
    """This class allows the storage and tracking of a list of nodes whose
    segments are to be used in the current calculation(s)"""
    def __init__(self, tree):
        """Initialize the Segment node list

        Parameters
        ----------
        tree : SegmentTree
            The segment tree from which the nodes listed will be drawn
        """
        self.tree = tree
        self.segments = [tree.head]
