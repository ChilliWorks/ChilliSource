#
#  AndroidBuildConfig.mk
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

#------------------------------------------------------------------------------
# This provides all of the standard build settings for a ChilliSource project.
# This shouldn't be included directly in most cases, AndroidBuildInitialise.mk 
# should be included instead.
#
# CS_PROJECT_ROOT must be set to the relative path to the root of the project
# prior to calling this.
#
# @author Ian Copland
#------------------------------------------------------------------------------

LOCAL_PATH = .

#store the location of the Get Files script
CS_SCRIPT_GETFILESWITHEXTENSIONS := $(CS_PROJECT_ROOT)/ChilliSource/Tools/Scripts/get_file_paths_with_extensions.py

#setup the module names.
CS_MODULENAME_CHILLISOURCE := ChilliSource-prebuilt
CS_MODULENAME_CSBASE := CSBase-prebuilt
CS_MODULENAME_CK := ck-prebuilt
CS_MODULENAME_APPLICATION := Application

#debug or release specific build flags.
ifeq ($(NDK_DEBUG),1)
CS_CXXFLAGS_TARGET := -g -DDEBUG -DCS_ENABLE_DEBUG -DCS_LOGLEVEL_VERBOSE
else
CS_CXXFLAGS_TARGET := -O3 -DNDEBUG -DCS_LOGLEVEL_WARNING
endif

#add in the appropriate android extensions
ifeq ($(CS_FLAVOUR_SKU), googleplay)
CS_CXXFLAGS_TARGET += -DCS_ANDROIDFLAVOUR_GOOGLEPLAY
else ifeq ($(CS_FLAVOUR_SKU), amazon)
CS_CXXFLAGS_TARGET += -DCS_ANDROIDFLAVOUR_AMAZON
endif

#setup warnings
CS_WARNINGS := -Wfatal-errors -Wchar-subscripts -Wcomment -Wnonnull -Winit-self -Wmissing-braces -Wmissing-include-dirs -Wparentheses -Wsequence-point -Wreturn-type -Wswitch -Wunused-function -Wunused -Wuninitialized -Wno-reorder

#setup build settings
CS_CXXFLAGS := -fsigned-char -std=c++11 -pthread -fexceptions -frtti -DCS_TARGETPLATFORM_ANDROID $(CS_WARNINGS) $(CS_CXXFLAGS_TARGET)
CS_STATIC_LIBRARIES := $(CS_MODULENAME_CSBASE) $(CS_MODULENAME_CK) $(CS_MODULENAME_CHILLISOURCE) cpufeatures
CS_LDLIBS := -lz -llog -lGLESv2 -lEGL
CS_C_INCLUDES := $(CS_PROJECT_ROOT)/ChilliSource/Source/ $(CS_PROJECT_ROOT)/ChilliSource/Libraries/Core/Android/Headers/ $(CS_PROJECT_ROOT)/ChilliSource/Libraries/CricketAudio/Android/Headers/
