#!/usr/bin/python
#
#  android_ndk_build.py
#  Chilli Source
#  Created by Ian Copland on 31/03/2015.
#
#  The MIT License (MIT)
#
#  Copyright (c) 2015 Tag Games Limited
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#  THE SOFTWARE.
#

import sys
import os
import platform
import subprocess
import tempfile
import hashlib

#------------------------------------------------------------------------------
# TODO!!!
# 
# @author Ian Copland
#
# @param ndk_directory_path - The absolute path to the directory that contains 
# the NDK.
# @param project_directory_path - The absolute path to the directory which 
# should be built.
# @param ndk_build_options - An array of options passed to ndk build.
#------------------------------------------------------------------------------
def ndk_build(ndk_directory_path, project_directory_path, ndk_build_options):
    previous_directory = os.getcwd()
    os.chdir(project_directory_path)

    #if we are on windows we need to change the build script we are calling
    executable = "ndk-build"
    if platform.system() == 'Windows':
        executable = "ndk-build.cmd"

    commands = [ os.path.join(ndk_directory_path, executable), "-C", project_directory_path ]

    #on windows we want to change the output directory to a much shorter path. This
    #should solve issues with path length.
    if platform.system() == 'Windows':
        dir_hash = hashlib.md5(project_directory_path.encode()).hexdigest()[0:10]
        temp_out = "C:/.CSAS/" + dir_hash
        if os.path.exists(temp_out) == False:
            os.makedirs(temp_out)
        commands.append("NDK_OUT=" + temp_out)

    commands.extend(ndk_build_options)
    subprocess.call(commands)

    os.chdir(previous_directory)
#------------------------------------------------------------------------------
# The entry point to the application. Parses the input and calls ndk_build if
# the input is valid.
# 
# @author Ian Copland
#------------------------------------------------------------------------------ 
if __name__ == "__main__":
    
    if len(sys.argv) < 3:
        print("Invalid number of parameters.")
        print("Usage: python android_ndk_build.py <ndk path> <build directory> [<ndk-build options>...]")

    ndk_directory_path = sys.argv[1]
    project_directory_path = sys.argv[2]
    ndk_build_options = sys.argv[3: len(sys.argv)]
    ndk_build(ndk_directory_path, project_directory_path, ndk_build_options)
