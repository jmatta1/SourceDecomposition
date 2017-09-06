"""This file contains miscellaneous utility functions and data"""

import sys
import math

USAGE_STR = """Usage:
    {0:s} <Number of Axial Pieces> <Number of Threads>
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
    <Number of Threads> must be an integer greater than or equal to 1

    It is the maximum number of simultaneous decompositions it can perform
"""
NUMTHREAD_ERR_BAD_PARSE = "<Number of Threads> was not a valid integer"
NUMTHREAD_ERR_SMALL_VAL = "<Number of Threads> too small"


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
    """
    if len(sys.argv) != 3:
        print USAGE_STR.format(sys.argv[0])
        print "Needs different arguments"
        sys.exit()
    # attempt to parse the parameters tell the user and exit if we can't
    num_segments = parse_and_validate_num_segs(sys.argv[1])
    # try to parse numThreads
    num_threads = parse_and_validate_num_threads(sys.argv[2])
    return num_segments, num_threads


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
            raise ValidationError("<Number of Threads> is less than 1")
    except ValidationError as err:
        print USAGE_STR.format(sys.argv[0])
        print NUMSEG_ERR.format(NUMTHREAD_ERR_SMALL_VAL)
        sys.exit()
    except ValueError:
        print USAGE_STR.format(sys.argv[0])
        print NUMTHREAD_ERR.format(NUMTHREAD_ERR_BAD_PARSE)
        sys.exit()
    except BaseException as err:
        print USAGE_STR.format(sys.argv[0])
        print "Unexpected error reading <Number of Threads>"
        print "Error was:\n\t", err
        sys.exit()
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
            raise ValidationError("Small Value")
        elif int(divs) != divs:
            raise ValidationError("Bad Exp")
    except ValidationError as err:
        print USAGE_STR.format(sys.argv[0])
        if err.args[0] == "Small Value":
            print NUMSEG_ERR.format(NUMSEG_ERR_SMALL_VAL)
        elif err.args[0] == "Bad Exp":
            print NUMSEG_ERR.format(NUMSEG_ERR_BAD_POW)
        else:
            print "Unknown error reading <Number of Axial Pieces>"
        sys.exit()
    except ValueError:
        print USAGE_STR.format(sys.argv[0])
        print NUMSEG_ERR.format(NUMSEG_ERR_BAD_PARSE)
        sys.exit()
    except BaseException as err:
        print USAGE_STR.format(sys.argv[0])
        print "Unexpected error reading <Number of Axial Pieces>"
        print "Error was:\n\t", err
        sys.exit()
    return num_segments
