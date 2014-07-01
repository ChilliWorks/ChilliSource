#!/usr/bin/python
#
#  copy_windows_resources.py
#  Chilli Source
#  Created by Scott Downie on 12/06/2014.
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
import subprocess
import shutil
import os

#----------------------------------------------------------------------
# Copies the resources from AppResources and CSResources
# into windows target directories
#
# Premulitplies all the PNGs
#
# @author S Downie
#----------------------------------------------------------------------

#----------------------------------------------------------------------
# Copies the files from src directory to dst directory but excludes
# those that are tagged ".ios" or ".android"
#
# @author S Downie
#
# @param Source path
# @param Destination path
#----------------------------------------------------------------------
def copy_file_tree(src_path, dst_path):
    excludes = [".ios", ".android", ".DS_Store"]
    includes = [".windows"]

    filter_func = lambda name: any(include in name for include in includes) or not any(exclude in name for exclude in excludes)

    if os.path.exists(dst_path) == False:
        os.makedirs(dst_path)

    for item in os.listdir(src_path):
        src = os.path.join(src_path, item)
        dst = os.path.join(dst_path, item)
        if os.path.isdir(src):
            copy_file_tree(src, dst)
        else:
            if filter_func(item):
                shutil.copy2(src, dst)

#----------------------------------------------------------------------
# Copies the resource from CSResources and AppResources
# into target directory.
#
# @author S Downie
#
# @param Project directory path
# @param Target directory path
#----------------------------------------------------------------------
def copy_resources(project_dir, target_dir):
    assetsDir = os.path.join(target_dir, "assets")

    file_system_utils.delete_directory(assetsDir)

    app_src_path = os.path.join(project_dir, "..", "..", "Content", "AppResources")
    cs_src_path = os.path.join(project_dir, "..", "..", "ChilliSource", "CSResources")

    app_dst_path = os.path.join(assetsDir, "AppResources")
    cs_dst_path = os.path.join(assetsDir, "CSResources")

    copy_file_tree(app_src_path, app_dst_path)
    copy_file_tree(cs_src_path, cs_dst_path)

#----------------------------------------------------------------------
# Copies the libs/dlls into target directory.
#
# @author S Downie
#
# @param Project directory path
# @param Target directory path
#----------------------------------------------------------------------
def copy_libs(project_dir, target_dir):
    libs_src_path = os.path.join(project_dir, "..", "..", "ChilliSource", "Libraries", "Core", "Windows", "Libs")
    dll_files = file_system_utils.get_file_paths_with_extensions(libs_src_path, ["dll"])

    for dll_file in dll_files:
        shutil.copy(dll_file, target_dir)

#----------------------------------------------------------------------
# Premulitplies all the PNGs in assets
#
# @author S Downie
#
# @param Project directory path
# @param Target directory path
#----------------------------------------------------------------------
def premultiply_pngs(project_dir, target_dir):
    assetsDir = os.path.join(target_dir, "assets")

    jarFile = os.path.join(project_dir, "..", "..", "ChilliSource", "Tools", "PNGAlphaPremultiplier.jar")
    png_files = file_system_utils.get_file_paths_with_extensions(assetsDir, ["png"])

    for png_file in png_files:
        subprocess.call(["java", "-Djava.awt.headless=true", "-Xmx512m", "-jar", jarFile, "--input", png_file, "--output", png_file]);

#----------------------------------------------------------------------
# The entry point into the script.
#
# @author S Downie
#
# @param The list of arguments.
#----------------------------------------------------------------------
def main(args):
    if not len(args) is 3:
        print("ERROR: Missing project path or target path")
        return

    project_dir = args[1]
    target_dir = args[2]

    copy_resources(project_dir, target_dir)
    copy_libs(project_dir, target_dir)
    premultiply_pngs(project_dir, target_dir)

if __name__ == "__main__":
    main(sys.argv)
