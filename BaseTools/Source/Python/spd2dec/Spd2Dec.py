## @file
# Convert an XML-based SPD file to a text-based DEC file.
#
# Copyright (c) 2007, Intel Corporation. All rights reserved.<BR>
# This program and the accompanying materials
# are licensed and made available under the terms and conditions of the BSD License
# which accompanies this distribution.  The full text of the license may be found at
# http://opensource.org/licenses/bsd-license.php
#
# THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
# WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#

##
# Import Modules
#
import sys
from Common.MigrationUtilities import *
from LoadSpd import LoadSpd
from StoreDec import StoreDec
from ConvertPackage import ConvertSpdPackageToDecPackage

## Entrance method
#
# This method mainly dispatch specific methods per the command line options.
# If no error found, return zero value so the caller of this tool can know
# if it's executed successfully or not.
#
# @retval 0     Tool was successful.
# @retval 1     Tool failed.
#
def Main():
    try:
        Options, InputFile = MigrationOptionParser("SPD", "DEC", "%prog")
        Package = LoadSpd(InputFile)
        ConvertSpdPackageToDecPackage(Package)
        StoreDec(Options.OutputFile, Package)
        return 0
    except Exception, e:
        print e
        return 1

if __name__ == '__main__':
    sys.exit(Main())


