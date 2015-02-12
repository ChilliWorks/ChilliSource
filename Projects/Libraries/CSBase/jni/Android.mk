#
#  Android.mk
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

GET_FILES_SCRIPT:=../../../Tools/Scripts/get_file_paths_with_extensions.py
LOCAL_PATH := $(call my-dir)

#get all the files to be build
AES_FILES := $(shell 'python' '$(GET_FILES_SCRIPT)' '-d' 'Source/aes' '-p' '../' '-e' 'cpp,c')
BASE64_FILES := $(shell 'python' '$(GET_FILES_SCRIPT)' '-d' 'Source/base64' '-p' '../' '-e' 'cpp,c')
JSON_FILES := $(shell 'python' '$(GET_FILES_SCRIPT)' '-d' 'Source/json' '-p' '../' '-e' 'cpp,c')
MD5_FILES := $(shell 'python' '$(GET_FILES_SCRIPT)' '-d' 'Source/md5' '-p' '../' '-e' 'cpp,c')
MINIZIP_FILES := $(shell 'python' '$(GET_FILES_SCRIPT)' '-d' 'Source/minizip' '-p' '../' '-e' 'cpp,c')
PNG_FILES := $(shell 'python' '$(GET_FILES_SCRIPT)' '-d' 'Source/png' '-p' '../' '-e' 'cpp,c')
RAPIDXML_FILES := $(shell 'python' '$(GET_FILES_SCRIPT)' '-d' 'Source/rapidxml' '-p' '../' '-e' 'cpp,c')
SHA1_FILES := $(shell 'python' '$(GET_FILES_SCRIPT)' '-d' 'Source/SHA1' '-p' '../' '-e' 'cpp,c')

#build the ChilliSource library
include $(CLEAR_VARS)
LOCAL_MODULE := CSBase
LOCAL_CXXFLAGS := -O3 -DNDEBUG -fsigned-char -std=c++11 -pthread -fexceptions -frtti
LOCAL_SRC_FILES := $(AES_FILES) $(BASE64_FILES) $(JSON_FILES) $(MD5_FILES) $(MINIZIP_FILES) $(PNG_FILES) $(RAPIDXML_FILES) $(SHA1_FILES)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../Source/
include $(BUILD_STATIC_LIBRARY)

