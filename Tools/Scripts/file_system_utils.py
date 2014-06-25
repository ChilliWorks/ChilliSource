#!/usr/bin/python
#
#  FileSystemUtils.py
#  Chilli Source
#  Created by Ian Copland on 20/08/2012.
#
#  The MIT License (MIT)
#
#  Copyright (c) 2012 Tag Games Limited
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
import shutil
import zipfile
import errno
from zipfile import ZipFile, ZIP_DEFLATED
import fnmatch

#----------------------------------------
# Calculates the absolute path from a path
# relative to this script file.
# 
# @author I Copland
#
# @param The path relative to this source
# file.
# @return The absolute path.
#----------------------------------------
def get_path_from_here(path):
    return os.path.realpath(os.path.join(os.path.dirname(os.path.realpath(__file__)), instrPath));
#----------------------------------------
# Overwrite the given dst directory or create
# it if it doesn't exist with the contents
# of the src directory
#
# @author S Downie
#----------------------------------------
def overwrite_directory(src_path, dst_path):
    delete_directory(dst_path)
    try:
        shutil.copytree(src_path, dst_path)
    except OSError as e:
        # If the error was caused because the source wasn't a directory
        if e.errno == errno.ENOTDIR:
            shutil.copy(src_path, dst_path)
        else:
            print('Directory not copied. Error: %s' % e)

#----------------------------------------
# Copy the given dst directory or create
# it if it doesn't exist with the contents
# of the src directory. This will overwrite
# exisiting files of the same name in the dst
# directory
#
# @author S Downie
#----------------------------------------        
def copy_directory(src_path, dst_path):
    if os.path.exists(dst_path) == False:
        os.makedirs(dst_path)

    for item in os.listdir(src_path):
        src = os.path.join(src_path, item)
        dst = os.path.join(dst_path, item)
        if os.path.isdir(src):
            copy_directory(src, dst)
        else:
            shutil.copy2(src, dst)
#----------------------------------------
# Deletes a directory if it exists. Does
# not return an error if trying to delete
# a directory that doesn't exist.
#
# @author I Copland
#
# @param The path to the directory to delete.
#----------------------------------------
def delete_directory(directory_path):
    if os.path.exists(directory_path) == True:
        shutil.rmtree(directory_path)
#----------------------------------------
# Deletes a file if it exists. Does not
# return an error if trying to delete a
# file that doesn't exist.
#
# @author I Copland
#
# @param The path to the file to delete.
#----------------------------------------
def delete_file(file_path):
    if os.path.isfile(file_path) == True:
        os.remove(file_path)
#----------------------------------------
# @author I Copland
#
# @param The path.
# @param The extension. This is not case
# sensitive.
# @return Whether or not the path has
# the given extension.
#----------------------------------------
def has_extension(file_path, extension):
    lowercase_filepath = file_path.lower()
    lowercase_extension = extension.lower()
    return lowercase_filepath.endswith(lowercase_extension)
#----------------------------------------
# Returns a list of all file paths with
# any of the given extensions in the
# requested directory. 
#
# @author I Copland
#
# @param The directory path.
# @param A list of possible extensions. This
# is not case sensitive.
#----------------------------------------
def get_file_paths_with_extensions(directory_path, extensions):
    output = []
    for curr_directory_path, child_directory_paths, file_paths in os.walk(directory_path):
        for file_path in file_paths:
            for extension in extensions:
                if has_extension(file_path, extension) == True:
                    output.append(os.path.join(curr_directory_path, file_path))
    return output

