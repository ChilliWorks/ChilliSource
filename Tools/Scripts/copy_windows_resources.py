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
# Copies the resource from PlatformResources and AppResources
# into target directory.
#
# @author S Downie
#
# @param Project directory path
# @param Target directory path
#----------------------------------------------------------------------
def copy_resources(project_dir, target_dir):
    assetsDir = target_dir+"assets/"

    file_system_utils.delete_directory(assetsDir)

    app_src_path = os.path.join(project_dir, "AppResources")
    cs_src_path = os.path.join(project_dir, "ChilliSource", "CSResources")

    app_dst_path = os.path.join(assetsDir, "AppResources")
    cs_dst_path = os.path.join(assetsDir, "CSResources")

    file_system_utils.overwrite_directory(app_src_path, app_dst_path)
    file_system_utils.overwrite_directory(cs_src_path, cs_dst_path)

#----------------------------------------------------------------------
# Copies the libs/dlls into target directory.
#
# @author S Downie
#
# @param Project directory path
# @param Target directory path
#----------------------------------------------------------------------
def copy_libs(project_dir, target_dir):
    libs_src_path = os.path.join(project_dir, "ChilliSource", "Libraries", "Core", "Libs", "Windows")
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

    jarFile = os.path.join(project_dir, "ChilliSource", "Tools", "PreMultipliedAlphaPNGTool.jar")
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
