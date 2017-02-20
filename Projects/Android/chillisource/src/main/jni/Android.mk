#
#  Android.mk
#  ChilliSource
#  Created by Ian Copland on 12/03/2015
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

#prepare the ChilliSource libraries
CS_PROJECT_ROOT=../../../../../../..
include $(CS_PROJECT_ROOT)/ChilliSource/Tools/Scripts/AndroidBuildConfig.mk

#gather all files in the engine that should be built
CS_SOURCEFILES_CHILLISOURCE := $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_PROJECT_ROOT)/ChilliSource/Source/ChilliSource/' '--extensions' 'cpp,c,cc')
CS_SOURCEFILES_PLATFORM := $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_PROJECT_ROOT)/ChilliSource/Source/CSBackend/Platform/Android/Main/JNI/' '--extensions' 'cpp,c,cc')
CS_SOURCEFILES_RENDERING := $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_PROJECT_ROOT)/ChilliSource/Source/CSBackend/Rendering/OpenGL/' '--extensions' 'cpp,c,cc')

#add files for appropriate android skus
ifeq ($(CS_FLAVOUR_SKU), googleplay)
CS_SOURCEFILES_PLATFORM +=$(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_PROJECT_ROOT)/ChilliSource/Source/CSBackend/Platform/Android/GooglePlay/JNI/' '--extensions' 'cpp,c,cc')
else ifeq ($(CS_FLAVOUR_SKU), amazon)
CS_SOURCEFILES_PLATFORM +=$(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_PROJECT_ROOT)/ChilliSource/Source/CSBackend/Platform/Android/Amazon/JNI/' '--extensions' 'cpp,c,cc')
endif

#build the ChilliSource static library
include $(CLEAR_VARS)
LOCAL_MODULE := ChilliSource
LOCAL_CXXFLAGS := $(CS_CXXFLAGS)
LOCAL_SRC_FILES := $(CS_SOURCEFILES_CHILLISOURCE) $(CS_SOURCEFILES_PLATFORM) $(CS_SOURCEFILES_RENDERING)
LOCAL_C_INCLUDES := $(CS_C_INCLUDES)
LOCAL_SHORT_COMMANDS := true
include $(BUILD_STATIC_LIBRARY)
