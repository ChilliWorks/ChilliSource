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

#----------------------------------------------------------------------
# Copies the resource from PlatformResources and AppResources
# into res and assets directories that are required by Android
#
# @author S Downie
#----------------------------------------------------------------------

#----------------------------------------
# The entry point into the script.
# 
# @author S Downie
#
# @param The list of arguments.
#----------------------------------------
def main(args):

    file_system_utils.delete_directory("../../../assets/")
    file_system_utils.delete_directory("../../../res/")

    app_src_path = "../../../AppResources/"
    cs_src_path = "../../CSResources/"
    platform_src_path = "../../../PlatformResources/Android/"

    #Dest dirs
    app_dst_path = "../../../assets/AppResources/"
    cs_dst_path = "../../../assets/CSResources/"
    platform_dst_path = "../../../res/"

    file_system_utils.overwrite_directory(app_src_path, app_dst_path)
    file_system_utils.overwrite_directory(cs_src_path, cs_dst_path)
    file_system_utils.overwrite_directory(platform_src_path, platform_dst_path)

if __name__ == "__main__":
    main(sys.argv)
