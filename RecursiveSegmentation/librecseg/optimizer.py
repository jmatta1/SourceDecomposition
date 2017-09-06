"""This file contains the basic optimizer class which attempts to find the
best wall segmentation that gives the best spatial reproduction while reducing
the count rate on the AD"""


import os
import multiprocessing as mp


class DecompOptimizer(object):
    """Class to perform position decomposition optimization on the wall
    segmentation"""
    def __init__(self, num_threads, data_dir):
        """Initializes a position decomposition optimizer

        Parameters
        ----------
        num_threads : int
            The number of threads that can be used for decompositions
        """
        # num_threads is definately between 1 and the number of cpus
        # so no further checks are needed
        self.pool = mp.Pool(processes=num_threads)
        self.out_queue = mp.Queue()
        for i in range(num_threads):
            out_fn = "decomp_out_{0:d}.root".format(i)
            self.out_queue.put(os.path.join(data_dir, out_fn))
        self.file_list = {}
        self.file_list["BaseNaIResp"] = os.path.join(data_dir, "NaIResp.root")
        self.file_list["AllNaIResp"] = os.path.join(data_dir,
                                                    "AllNaIResp.root")
        self.file_list["BasePanelResp"] = os.path.join(data_dir,
                                                       "PanelResp.root")
        self.file_list["AllPanelResp"] = os.path.join(data_dir,
                                                      "AllPanelResp.root")
        self.file_list["RawData"] = os.path.join(data_dir,
                                                 "ScanData.root")
        self.best_fit_list = []

    def perform_optimization(self, wall_list):
        """This is the function that starts performing the heavy lifting to
        try and find the right segment combination

        Parameters
        ----------
        wall_list : list of NodeList's
            The list of the node lists for each wall
        """
        pass

    def get_best_fit(self):
        """This function checks if the optimization has been run, if not it
        runs the optimization and returns the best fit function list, if it has
        been run then it simply returns the best fit optimization list

        Returns
        -------
        best_fit_funcs : list of str
            The list of response function names that minimizes the chi square
            in the reproduction of the data
        """
        if len(self.best_fit_list) == 0:
            self.perform_optimization()
        return self.best_filt_list
