#!/usr/bin/python
#
#  get_file_paths_with_extensions.py
#  Chilli Source
#  Created by Ian Copland on 28/05/2014.
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

import sys
import file_system_utils

#----------------------------------------
# The entry point into the script.
# 
# @author I Copland
#
# @param The list of arguments.
#----------------------------------------
def main(args):

    directory_path = ""
    directory_prefix = ""
    extensions = []

    argCount = len(args)
    for i in range(0, argCount):
        arg = args[i]
        
        if (arg == "--directory" or arg == "-d") and i+1 < argCount:
            directory_path = args[i+1]
        if (arg == "--prefix" or arg == "-p") and i+1 < argCount:
            directory_prefix = args[i+1]
        if (arg == "--extensions" or arg == "-e") and i+1 < argCount:
            extensions = args[i+1].split(",")

    file_paths = file_system_utils.get_file_paths_with_extensions(directory_path, extensions)

    output = ""
    first = True
    for file_path in file_paths:
        if first == False:
            output += " "
        first = False
        output += directory_prefix + file_path

    print(output)

if __name__ == "__main__":
    main(sys.argv)
