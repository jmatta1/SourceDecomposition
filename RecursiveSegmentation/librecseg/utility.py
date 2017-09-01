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
    # try to parse numSegments
    num_segments = 0
    try:
        num_segments = int(sys.argv[1])
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
        sys.exit()
    # try to parse numThreads
    num_threads = 0
    try:
        num_threads = int(sys.argv[2])
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
    except:
        print USAGE_STR.format(sys.argv[0])
        print "Unexpected error reading <Number of Threads>"
        sys.exit()
    return num_segments, num_threads
