"""This file contains the class allowing trees of summed segments to be built
"""

from librecseg.segment_tree_support import SegmentTreeNode, SegmentInfo, \
                                           gen_quartet_names

NAME_SUFFIX = "_{0:d}_{1:d}_{2:d}"

ADJ_OFFS = [[((1, 0), (0, 1)), ((-1, 0), (1, 0), (0, 1)), ((-1, 0), (0, 1))],
            [((1, 0), (0, -1), (0, 1)), ((-1, 0), (1, 0), (0, -1), (0, 1)),
             ((-1, 0), (0, -1), (0, 1))],
            [((1, 0), (0, -1)), ((-1, 0), (1, 0), (0, -1)), ((-1, 0), (0, -1))]
           ]


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
        # First make a list of the leaf segments
        self.build_leaves(base, divs)
        # Now, for each level above the leaves make the quartet summed segments
        self.build_branches(base, divs)
        # set the head node
        fmt_str = base + NAME_SUFFIX
        self.head = self.seg_dict[fmt_str.format(1, 0, 0)].get_node()

    def build_branches(self, base, divs):
        """Build the branch nodes of the SegmentTree

        Parameters
        ----------
        base : str
            The base name of functions in the root file
        divs : int
            The number of divisions per axis of the wall, must be a power of 2
        """
        fmt_str = base + NAME_SUFFIX
        curr_segs = divs/2
        while curr_segs > 0:
            # for each set of segment combo in the curr_segs subdivision of the
            # wall, construct the segment and add it
            for i in range(curr_segs):
                for j in range(curr_segs):
                    # make the name
                    name = fmt_str.format(curr_segs, i, j)
                    # make the composition
                    comp = [self.seg_dict[cname].get_node() for cname in
                            gen_quartet_names(fmt_str, curr_segs, (i, j))]
                    # make the node
                    node = SegmentTreeNode(name, True, False)
                    # add the composition
                    node.add_composition(comp)
                    # since we are not a leaf node, make the node generate its
                    # own adjacency list
                    node.build_adjacency()
                    # add the node as the parent to the nodes it is made of
                    for child_node in comp:
                        child_node.add_parent(node)
                    # make the segment info structure
                    info = SegmentInfo(True, node)
                    # insert the segment info into the segment dictionary
                    self.seg_dict[name] = info
            curr_segs /= 2
        fmt_str = base + NAME_SUFFIX

    def build_leaves(self, base, divs):
        """Build the leaf nodes of the SegmentTree

        Parameters
        ----------
        base : str
            The base name of functions in the root file
        divs : int
            The number of divisions per axis of the wall, must be a power of 2
        """
        fmt_str = base + NAME_SUFFIX
        lookup = [0] + [1]*(divs-2) + [2]
        for i in range(divs):
            for j in range(divs):
                # make the name
                name = fmt_str.format(divs, i, j)
                # create the node
                node = SegmentTreeNode(name, True, True)
                # add the node to the node list
                info = SegmentInfo(True, node)
                self.seg_dict[name] = info
        # now generate the adjacency lists, we did not do this earlier because
        # the adjacent nodes did not exist yet
        for i in range(divs):
            for j in range(divs):
                node = self.seg_dict[fmt_str.format(divs, i, j)].get_node()
                for ioff, joff in ADJ_OFFS[lookup[j]][lookup[i]]:
                    adj_name = fmt_str.format(divs, i + ioff, j + joff)
                    adj = self.seg_dict[adj_name].get_node()
                    node.add_adjacent_node(adj)

    def print_tree(self):
        """Prints the tree structure in some visually reasonable manner"""
        self.head.print_self_and_children("", "")

    def get_node(self, func_name):
        """Returns the node corresponding to the function name

        Parameters
        ----------
        func_name : str
            Name of the function string whose node is to be looked up

        Returns
        -------
        func_node : SegmentTreeNode
            Node object that represents the given string
        """
        try:
            return self.seg_dict[func_name]
        except KeyError:
            return None
