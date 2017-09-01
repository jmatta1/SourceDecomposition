"""This file contains the class allowing trees of summed segments to be built
"""

from librecseg.segment_tree_support import SegmentTreeNode, SegmentInfo, \
                                           gen_quartet_names

NAME_SUFFIX = "_{0:d}_{1:d}_{2:d}"


class SegmentTree(object):
    """Class to hold a tree comprised of SegmentTreeNodes, initializes itself
    using the base name of the segments and a number of sub-divisions per axis
    """
    def __init__(self, base, divs):
        """Initialize the segment tree from the base name for the segments and
        the number of segments per axis

        Parameters
        ----------
        base : str
            The base name of functions in the root file

        divs : int
            The number of divisions per axis of the wall, must be a power of 2
        """
        self.name_base = base
        self.max_segs = divs
        self.head = None
        self.seg_dict = {}
        fmt_str = base + NAME_SUFFIX
        # First make a list of the leaf segments
        for i in range(divs):
            for j in range(divs):
                name = fmt_str.format(divs, i, j)
                node = SegmentTreeNode(name, True, (divs, i, j))
                info = SegmentInfo(True, node)
                self.seg_dict[name] = info
        # Now, for each level above the leaves make the quartet summed segments
        curr_segs = divs/2
        while curr_segs > 0:
            for i in range(curr_segs):
                for j in range(curr_segs):
                    name = fmt_str.format(curr_segs, i, j)
                    comp = [self.seg_dict[cname].get_node() for cname in
                            gen_quartet_names(fmt_str, curr_segs, (i, j))]
                    node = SegmentTreeNode(name, True, (curr_segs, i, j))
                    node.add_composition(comp)
                    for child_node in comp:
                        child_node.add_parent(node)
                    info = SegmentInfo(True, node)
                    self.seg_dict[name] = info
            curr_segs /= 2
        self.head = self.seg_dict[fmt_str.format(1,0,0)].get_node()

    def print_tree(self):
        """Prints the tree structure in some visually reasonable manner"""
        self.head.print_self_and_children("", "")

    def test_adjacency(self, func1, func2):
        """Takes two function names and tests if they are adjacent to each
        other

        Parameters
        ----------
        func1 : str
            Name of the first function to be tested
        func2 : str
            Name of the second function to be tested

        Returns
        -------
        are_adjacent : bool
            True if the two functions have at least one segment or sub-segment
            adjacent to eachother, False otherwise
        """
        node1 = self.seg_dict[func1].get_node()
        node2 = self.seg_dict[func2].get_node()
        return node1.is_adjacent(node2)
