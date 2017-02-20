#
#  AndroidBuildInitialise.mk
#  ChilliSource
#  Created by Ian Copland on 12/03/2015.
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

#------------------------------------------------------------------------------
# This copies the required static libraries to the currently built project. 
# The android build settings are also included, so this is the only thing that
# a makefile building a ChilliSource shared library will need to include. 
# 
# Prior to including this, CS_PROJECT_ROOT must be set to the relative path to
# the root of the project, i.e, the directory ChilliSource is located in.
#
# @author Ian Copland
#------------------------------------------------------------------------------

include $(call my-dir)/AndroidBuildConfig.mk

ifndef CS_CLEAN

#copy pre-built static libraries
include $(CLEAR_VARS)
LOCAL_MODULE := $(CS_MODULENAME_CSBASE)
LOCAL_SRC_FILES := $(CS_PROJECT_ROOT)/ChilliSource/Libraries/Core/Android/Libs/$(TARGET_ARCH_ABI)/libCSBase.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := $(CS_MODULENAME_CK)
LOCAL_SRC_FILES := $(CS_PROJECT_ROOT)/ChilliSource/Libraries/CricketAudio/Android/Libs/$(TARGET_ARCH_ABI)/libck.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := $(CS_MODULENAME_CHILLISOURCE)
LOCAL_SRC_FILES := $(CS_PROJECT_ROOT)/ChilliSource/Projects/Android/chillisource/src/main/libs/$(TARGET_ARCH_ABI)/libChilliSource.a
include $(PREBUILT_STATIC_LIBRARY)

endif

