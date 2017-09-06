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
        self.seg_rates = [0]

    def get_tree_name(self):
        """Gets the name of the tree that this nodeList is handling

        Returns
        -------
        tree_name : str
            Base name of the tree
        """
        return self.tree.name_base

    def get_segs_in_use(self):
        """Gets the number of segments for current usage

        Returns
        -------
        num_segments : int
            The number of segments currently in the segment list
        """
        return len(self.segments)
