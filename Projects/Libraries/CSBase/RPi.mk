#  The MIT License (MIT)
#
#  Copyright (c) 2017 Tag Games Limited
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
#-----------------------------------------------------------------------------------

# CSBase makefile for RPi

LOCAL_PATH = .

# Project root
CS_PROJECT_ROOT=../../../..

# Python script
CS_SCRIPT_GETFILESWITHEXTENSIONS = $(CS_PROJECT_ROOT)/ChilliSource/Tools/Scripts/get_file_paths_with_extensions.py

# Set up CSBase root.
SOURCEDIR= $(LOCAL_PATH)/Source

# Build directory
BUILDDIR= $(LOCAL_PATH)/build/RPi

# Gather files to build
CSBASE_SOURCES := $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(SOURCEDIR)/' '--extensions' 'cpp,c,cc')

# Set up tools.
CC=/Volumes/xtools/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-g++
CCO=/Volumes/xtools/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-gcc
AR=/Volumes/xtools/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-ar

# Compiler Flags
CS_CXXFLAGS_TARGET=-DDEBUG -DCS_ENABLE_DEBUG -DCS_LOGLEVEL_VERBOSE -DCS_TARGETPLATFORM_RPI -DCS_OPENGLVERSION_ES

# Includes
CS_INCLUDES= -I$(SOURCEDIR) -I$(LOCAL_PATH)/PlatformSource/RPi

# Compiler Flags
CFLAGS=-c -O3 -std=c++11 $(CS_CXXFLAGS_TARGET) $(CS_INCLUDES)

# Source files
SOURCES=$(CSBASE_SOURCES)

# All Objects to be Generated - they take their names from the names of the cpp files that generated them.
OBJECTS= $(SOURCES:%.cpp=%.o) $(SOURCES:%.c=%.o) $(SOURCES:%.cc=%.o)

# Name of static lib to generate.
CSBASE_STATIC_LIB=libCSBase.a

# Default make command.
all: $(SOURCES) $(CSBASE_STATIC_LIB)

# Link objs into static lib. Uses the .cpp.o: rule below.
$(CSBASE_STATIC_LIB): $(OBJECTS)
	$(AR) rcs $(BUILDDIR)/$(CSBASE_STATIC_LIB) $(OBJECTS)

# Create objects. Using $(OBJECTS) as a rule is shorthand for running this on all cpp files in $(SOURCES).
# $< refers to the first prerequisite, which is $(SOURCES). $@ refers to the target, which is $(OBJECTS)
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
.c.o:
	$(CCO) $(CFLAGS) $< -o $@
.cc.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(SOURCEDIR)/' '--extensions' 'o')