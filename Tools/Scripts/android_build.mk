#
#  android_build.mk
#  Chilli Source
#  Created by Ian Copland on 19/06/2014.
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

#set the local path to the root of the project.
LOCAL_PATH := $(call my-dir)/../../../Projects/Android

#get the path to the script file used to get all file path in directory
CS_SCRIPT_GETFILESWITHEXTENSIONS := ../../ChilliSource/Tools/Scripts/get_file_paths_with_extensions.py

#setup the warnings that should be used when compiling
CS_WARNINGS_USE := -Wfatal-errors -Wchar-subscripts -Wcomment -Wnonnull -Winit-self -Wmissing-braces -Wmissing-include-dirs -Wparentheses -Wsequence-point -Wreturn-type -Wswitch -Wunused-function -Wunused -Wuninitialized
CS_WARNINGS_DONTUSE := -Wno-reorder

#setup the module names.
CS_MODULENAME_CHILLISOURCE := ChilliSource
CS_MODULENAME_CSBASE := CSBase-prebuilt
CS_MODULENAME_CK := ck-prebuilt
CS_MODULENAME_OUTPUT := Application

#setup the build parameters
CS_CXXFLAGS := -fsigned-char -std=c++11 -pthread -fexceptions -frtti -DCS_TARGETPLATFORM_ANDROID $(CS_WARNINGS_USE) $(CS_WARNINGS_DONTUSE) $(CS_CXXFLAGS_TARGET) $(CS_CXXFLAGS_EXTRA)
CS_WHOLE_STATIC_LIBRARIES := $(CS_MODULENAME_CSBASE) $(CS_MODULENAME_CK) $(CS_MODULENAME_CHILLISOURCE) cpufeatures
CS_LDLIBS := -fuse-ld=gold -lz -llog -lGLESv2
CS_C_INCLUDES := ../../ChilliSource/Source/ ../../ChilliSource/Libraries/Core/Android/Headers/ ../../ChilliSource/Libraries/CricketAudio/Android/Headers/

#gather all files in the engine that should be built
CS_SOURCEFILES_CHILLISOURCE := $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '../../ChilliSource/Source/ChilliSource/' '--extensions' 'cpp,c,cc')
CS_SOURCEFILES_PLATFORM := $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '../../ChilliSource/Source/CSBackend/Platform/Android/' '--extensions' 'cpp,c,cc')
CS_SOURCEFILES_RENDERING := $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '../../ChilliSource/Source/CSBackend/Rendering/OpenGL/' '--extensions' 'cpp,c,cc')

#copy pre-built static libraries
include $(CLEAR_VARS)
LOCAL_MODULE := $(CS_MODULENAME_CSBASE)
LOCAL_SRC_FILES := ../../ChilliSource/Libraries/Core/Android/Libs/$(TARGET_ARCH_ABI)/libCSBase.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := $(CS_MODULENAME_CK)
LOCAL_SRC_FILES := ../../ChilliSource/Libraries/CricketAudio/Android/Libs/$(TARGET_ARCH_ABI)/libck.a
include $(PREBUILT_STATIC_LIBRARY)

#build the ChilliSource static library
include $(CLEAR_VARS)
LOCAL_MODULE := $(CS_MODULENAME_CHILLISOURCE)
LOCAL_CXXFLAGS := $(CS_CXXFLAGS)
LOCAL_SRC_FILES := $(CS_SOURCEFILES_CHILLISOURCE) $(CS_SOURCEFILES_PLATFORM) $(CS_SOURCEFILES_RENDERING)
LOCAL_C_INCLUDES := $(CS_C_INCLUDES)
include $(BUILD_STATIC_LIBRARY)

