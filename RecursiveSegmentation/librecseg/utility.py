"""This file contains miscellaneous utility functions and data"""

import sys
import math
import os
import multiprocessing as mp

USAGE_STR = """Usage:
    {0:s} <Number of Axial Pieces> <Number of Threads> <Data Directory>
"""

NUMSEG_ERR = """  Error:
    {0:s}
    <Number of Axial Pieces> must be an integer greater than or equal to 2 and
    must also be a power of 2

    It describes the number of pieces the walls have been divided into on each
    of the two axes.
"""
NUMSEG_ERR_BAD_PARSE = "<Number of Axial Pieces> was not a valid integer"
NUMSEG_ERR_SMALL_VAL = "<Number of Axial Pieces> was too small"
NUMSEG_ERR_BAD_POW = "<Number of Axial Pieces> not a power of 2"

NUMTHREAD_ERR = """  Error:
    {0:s}
    <Number of Threads> must be an integer greater than or equal to 1 and less
    than or equal to the number of CPUs in the machine

    It is the maximum number of simultaneous decompositions it can perform
"""
NUMTHREAD_ERR_BAD_PARSE = "<Number of Threads> was not a valid integer"
NUMTHREAD_ERR_SMALL_VAL = "<Number of Threads> too small"
NUMMTHREAD_ERR_BIG_VAL = "<Number of Threads> exceeds number of CPUs (={0:d})"

DATA_DIR_ERR = """  Error:
    {0:s} does not exist
    <Data Directory> must be the path to the directory which contains the
    following files:
      NaIResp.root - The file with the response functions for the smallest
                     subdivisions of the walls acting on the NaI detectors
      AllNaIResp.root - The file that contains all response functions for the
                        smalled subdivisions of the wall, the quartet summed
                        subdivisions of the wall, and will contain all
                        generated subdivisions of the wall acting on the NaI
                        detectors
      PanelResp.root - The file with the response functions for the smallest
                       subdivisions of the walls acting on the AD panels
      AllPanelResp.root - The file with the response functions for the smallest
                          subdivisions of the wall, the quartet summed
                          subdivisions of the wall, and will contain all
                          generated subdivisions of the wall acting on the Nai
                          detectors
      ScanData.root - The position scan spectra in the format needed by the
                      PositionDecomposer executable
"""

FILE_NAMES = ["NaIResp.root", "AllNaIResp.root", "PanelResp.root",
                  "AllPanelResp.root", "ScanData.root"]

class ValidationError(StandardError):
    """Simple custom exception class for sanity sake"""
    pass


def parse_and_validate_cmd_line():
    """Reads the command-line parameters checks to make sure they seem right
    and returns them

    Returns
    -------
    num_segments : int
        The number of segments per axis at the lowest level of division
    num_threads : int
        The number of threads that can be used in execution of decomps
    data_directory : str
        the path to the directory containing NaIResp.root, AllNaIResp.root,
        PanelResp.root, and AllPanelResp.root.
    """
    if len(sys.argv) != 4:
        print USAGE_STR.format(sys.argv[0])
        sys.exit()
    # attempt to parse the parameters tell the user and exit if we can't
    num_segments = parse_and_validate_num_segs(sys.argv[1])
    # try to parse numThreads
    num_threads = parse_and_validate_num_threads(sys.argv[2])
    # try to parse and test the data directory
    data_dir = parse_and_validate_data_dir(sys.argv[3])
    return num_segments, num_threads, data_dir


def parse_and_validate_data_dir(data_dir):
    """This function tests for the existence of the proper files in this
    directory

    Parameters
    ----------
    data_dir : str
        Path to the directory containing the promised root input files

    Returns
    -------
    data_dir : str
        if directory contained the correct files then return that directory
        otherwise call sys.exit() and never return
    """
    paths = [data_dir] + [os.path.join(data_dir, name) for name in FILE_NAMES]
    exists = [(path, os.path.exists(path)) for path in paths]
    try:
        for path, exist in exists:
            if not exist:
                raise ValidationError(path)
    except ValidationError as err
        print USAGE_STR.format(sys.argv[0])
        print DATA_DIR_ERR.format(err.args[0])
        sys.exit()


def parse_and_validate_num_threads(thread_str):
    """Attempts to parse the number of threads passed in on the command line

    Parameters
    ----------
    thread_str : str
        A string that should represent the number of threads to use

    Returns
    -------
    num_threads : int
        The integer representation of the number of threads to use
    """
    num_threads = 0
    try:
        num_threads = int(thread_str)
        if num_threads < 1:
            raise ValidationError(NUMTHREAD_ERR_SMALL_VAL)
        elif num_threads > mp.cpu_count()
            err_str = NUMMTHREAD_ERR_BIG_VAL.format(mp.cpu_count())
            raise ValidationError(err_str)
    except ValidationError as err:
        print USAGE_STR.format(sys.argv[0])
        NUMTHREAD_ERR.format(err.args[0])
        sys.exit()
    except ValueError:
        print USAGE_STR.format(sys.argv[0])
        print NUMTHREAD_ERR.format(NUMTHREAD_ERR_BAD_PARSE)
        sys.exit()
    except BaseException as err:
        print USAGE_STR.format(sys.argv[0])
        print NUMTHREAD_ERR.format("Unexpected error")
        print "Error was:\n\t", err
    return num_threads


def parse_and_validate_num_segs(segment_str):
    """Attempts to parse the number of segments passed in on the command line

    Parameters
    ----------
    segment_str : str
        A string that should represent the number of segments each axis of the
        walls was divided into

    Returns
    -------
    num_segments : int
        The integer representation of the number of segments each axis was
        divided into
    """
    # try to parse numSegments
    num_segments = 0
    try:
        num_segments = int(segment_str)
        divs = math.log(num_segments, 2)
        if num_segments < 2:
            raise ValidationError(NUMSEG_ERR_SMALL_VAL)
        elif int(divs) != divs:
            raise ValidationError(NUMSEG_ERR_BAD_POW)
    except ValidationError as err:
        print USAGE_STR.format(sys.argv[0])
        print NUMSEG_ERR.format(err.args[0])
        sys.exit()
    except ValueError:
        print USAGE_STR.format(sys.argv[0])
        print NUMSEG_ERR.format(NUMSEG_ERR_BAD_PARSE)
        sys.exit()
    except BaseException as err:
        print USAGE_STR.format(sys.argv[0])
        print NUMSEG_ERR.format("Unexpected error")
        print "Error was:\n\t", err
        sys.exit()
    return num_segments
