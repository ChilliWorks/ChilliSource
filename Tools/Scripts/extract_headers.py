#!/usr/bin/python
#
#  extract_headers.py
#  Chilli Source
#  Created by Ian Copland on 13/06/2014.
#
#  The MIT License (MIT)
#
#  Copyright (c) 2014 Tag Games Limited
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

#----------------------------------------------------------------------
# Copies all header files in the given source directory and places them
# in the output
#
# The parameters to the tool are:
#
#    '--input' or '-i'   -> The input directory path to look for headers.
#    '--output' or '-o'  -> The output directory whether the extracted
#                           headers will be placed.
#
# @author I Copland
#----------------------------------------------------------------------

import sys
import os
import shutil
import file_system_utils

#----------------------------------------------------------------------
# Extracts the headers.
#
# @author I Copland
#
# @param The input header directory path.
# @param The output header directory path.
#----------------------------------------------------------------------
def extract(input_directory_path, output_directory_path):

    #create the output directory.
    print("Creating directory '" + output_directory_path + "'")
    file_system_utils.delete_directory(output_directory_path)
    os.makedirs(output_directory_path)

    #copy all headers
    header_file_paths = file_system_utils.get_file_paths_with_extensions(input_directory_path, ["h", "hpp", "inl"])
    for header_file_path in header_file_paths:
        relative_header_file_path = header_file_path[len(input_directory_path):len(header_file_path)];
        input_header_file_path = os.path.join(input_directory_path, relative_header_file_path)
        output_header_file_path = os.path.join(output_directory_path, relative_header_file_path)
        print("Copying header '" + input_header_file_path + "' to " + output_header_file_path + "'")
        if os.path.exists(os.path.dirname(output_header_file_path)) == False:
            os.makedirs(os.path.dirname(output_header_file_path))
        shutil.copy2(input_header_file_path, output_header_file_path)

#----------------------------------------------------------------------
# The entry point into the script.
#
# @author I Copland
#
# @param The list of arguments.
#----------------------------------------------------------------------
def main(args):
    input_directory_path = ""
    output_directory_path = ""

    argCount = len(args)
    for i in range(0, argCount):
        arg = args[i]

        if (arg == "--input" or arg == "-i") and i+1 < argCount:
            input_directory_path = args[i+1]
        if (arg == "--output" or arg == "-o") and i+1 < argCount:
            output_directory_path = args[i+1]

    if len(input_directory_path) > 0 and len(output_directory_path) > 0:
        extract(input_directory_path, output_directory_path)
    else:
        print("Incorrect arguments supplied.")

if __name__ == "__main__":
    main(sys.argv)
