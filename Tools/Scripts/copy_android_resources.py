#!/usr/bin/python
#
#  copy_android_resources.py
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
import os
import shutil

#------------------------------------------------------------------------------
# Copies the resource from CSResources and AppResources into the assets 
# directory as required by Android. Any PNGs will also be pre-multiplied when
# copied.
#
# This will also build the AndroidManifest.xml from the provided
# CSAndroidManifest.xml file.
#
# The script takes two arguments: 
#  1) The root of the ChilliSource project.
#  2) The android project directory. i.e, the directory that contains the
#     AndroidManifest.xml and assets/.
#
# @author Ian Copland
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Copies the files from src directory to dst directory but excludes
# those that are tagged ".ios" or ".windows"
#
# @author S Downie
#
# @param Source path
# @param Destination path
#------------------------------------------------------------------------------
def copy_file_tree(src_path, dst_path):
    excludes = [".ios", ".windows", ".DS_Store"]
    includes = [".android"]

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

#------------------------------------------------------------------------------
# Copies the resource from ProjectResources, CSResources and AppResources
# into res and assets directories that are required by Android.
#
# @author S Downie
#
# @param The path to the root of the ChilliSource project.
# @param The path to the android project.
#------------------------------------------------------------------------------
def copy_resources(project_root_dir, project_android_dir):
    file_system_utils.delete_directory(os.path.join(project_android_dir, "assets/"))

    app_src_path = os.path.join(project_root_dir, "Content/AppResources/")
    cs_src_path = os.path.join(project_root_dir, "ChilliSource/CSResources/")

    app_dst_path = os.path.join(project_android_dir, "assets/AppResources/")
    cs_dst_path = os.path.join(project_android_dir, "assets/CSResources/")

    copy_file_tree(app_src_path, app_dst_path)
    copy_file_tree(cs_src_path, cs_dst_path)

#------------------------------------------------------------------------------
# Premulitplies all the PNGs in assets
#
# @author S Downie
#
# @param The path to the root of the ChilliSource project.
# @param The path to the android project.
#------------------------------------------------------------------------------
def premultiply_pngs(project_root_dir, project_android_dir):
    jarFile = os.path.join(project_root_dir, "ChilliSource/Tools/PNGAlphaPremultiplier.jar");
    png_files = file_system_utils.get_file_paths_with_extensions(os.path.join(project_android_dir, "assets/"), ["png"])

    for png_file in png_files:
        subprocess.call(["java", "-Djava.awt.headless=true", "-Xmx512m", "-jar", jarFile, "--input", png_file, "--output", png_file]);

#------------------------------------------------------------------------------
# Builds the android manifest.
#
# @author Ian Copland
#
# @param The path to the root of the ChilliSource project.
# @param The path to the android project.
#------------------------------------------------------------------------------
def build_manifest(project_root_dir, project_android_dir):
    jarFile = os.path.join(project_root_dir, "ChilliSource/Tools/AndroidManifestBuilder.jar")
    userManifest = os.path.join(project_android_dir, "CSAndroidManifest.xml")
    templateManifest = os.path.join(project_root_dir, "ChilliSource/Tools/Scripts/AndroidManifestTemplate.xml")
    outputManifest = os.path.join(project_android_dir, "AndroidManifest.xml")
    subprocess.call(["java", "-Djava.awt.headless=true", "-Xmx512m", "-jar", jarFile, "--input", userManifest, "--template", templateManifest, "--output", outputManifest]);

#------------------------------------------------------------------------------
# The entry point into the script.
#
# @author S Downie
#
# @param The list of arguments.
#------------------------------------------------------------------------------
def main(args):
    if not len(args) is 3:
        print("ERROR: Incorrect arguments. The script takes two arguments: ")
        print("  1) The root of the ChilliSource project.")
        print("  2) The android project directory. i.e, the directory that contains the AndroidManifest.xml and assets/.") 
        return

    project_root_dir = args[1]
    project_android_dir = args[2]
    copy_resources(project_root_dir, project_android_dir)
    premultiply_pngs(project_root_dir, project_android_dir)
    build_manifest(project_root_dir, project_android_dir)

if __name__ == "__main__":
    main(sys.argv)
