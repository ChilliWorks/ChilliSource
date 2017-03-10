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

LOCAL_PATH = .

# Set up project root.
CS_PROJECT_ROOT=../../../..
CS_APP_SRC_ROOT = $(CS_PROJECT_ROOT)/AppSource

# Set up scripts.
CS_SCRIPT_GETFILESWITHEXTENSIONS = $(CS_PROJECT_ROOT)/ChilliSource/Tools/Scripts/get_file_paths_with_extensions.py

# Gather all files in the engine that should be built.
CS_APP_SOURCES := $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_APP_SRC_ROOT)/' '--extensions' 'cpp,c,cc')

# Set up tools.
CC=/Volumes/xtools/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-g++
LD=/Volumes/xtools/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-g++
AR=/Volumes/xtools/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-ar

# Compiler Flags
CS_CXXFLAGS_TARGET=-DDEBUG -DCS_ENABLE_DEBUG -DCS_LOGLEVEL_VERBOSE -DCS_TARGETPLATFORM_RPI

# Includes
CS_INCLUDES=-I$(CS_PROJECT_ROOT)/ChilliSource/Libraries/Core/RPi/Headers -I$(CS_PROJECT_ROOT)/AppSource -I$(CS_PROJECT_ROOT)/Libraries/Catch/include  -I$(CS_PROJECT_ROOT)/ChilliSource/Source -I$(CS_PROJECT_ROOT)/ChilliSource/Libraries/CricketAudio/RPi/Headers

# Additional Compiler Flags
CFLAGS=-c -g -std=c++11 -fsigned-char -pthread -fexceptions -frtti $(CS_CXXFLAGS_TARGET) $(CS_INCLUDES)

# Library Directories
CS_LIBRARY_DIRS=-L$(CS_PROJECT_ROOT)/ChilliSource/Libraries/Core/RPi/Libs -L$(CS_PROJECT_ROOT)/ChilliSource/Libraries/CricketAudio/RPi/Libs -L$(LOCAL_PATH)

# Linker Flags
LDFLAGS= $(CS_LIBRARY_DIRS) -lApplication -lChilliSource -lCSBase -lvcos -lbcm_host -lGLESv2 -lEGL -lvchiq_arm -lrt -lm -lc -lstdc++ -l:libgcc_s.so.1 -l:libX11.so.6.3.0 -l:libXau.so.6.0.0 -l:libXdmcp.so.6.0.0 -l:libxcb.so.1.1.0 -l:libxcb-xkb.so.1.0.0 -l:libxkbcommon.so.0.0.0 -l:libxkbcommon-x11.so.0.0.0

# All Source Files
SOURCES=$(CS_APP_SOURCES)

# All Objects to be Generated - they take their names from the names of the cpp files that generated them.
CS_APP_OBJ_DIR = $(LOCAL_PATH)/appobj
OBJECTS := $(patsubst $(CS_APP_SRC_ROOT)/%.cpp, $(CS_APP_OBJ_DIR)/%.o, $(filter %.cpp, $(SOURCES))) $(patsubst $(CS_APP_SRC_ROOT)/%.c, $(CS_APP_OBJ_DIR)/%.o,  $(filter %.c, $(SOURCES))) $(patsubst $(CS_APP_SRC_ROOT)/%.cc, $(CS_APP_OBJ_DIR)/%.o, $(filter %.cc, $(SOURCES)))

# Name of static lib to link.
CS_STATIC_LIB=libChilliSource.a

# Name of application to generate
CS_APP_STATIC=libApplication.a
CS_APP_EXECUTABLE=Application

# Default make command.
all: $(SOURCES) $(CS_APP_EXECUTABLE)

# Link objs into static lib. Uses the .cpp.o: rule below.
$(CS_APP_EXECUTABLE): $(CS_APP_STATIC)
	@echo "Building CS_APP_EXECUTABLE"
	$(LD) $(OBJECTS) $(LDFLAGS) -o $@

# Archive all app objects together to be a static lib
$(CS_APP_STATIC): $(OBJECTS)
	@echo "Building CS_APP_STATIC"
	$(AR) rcs $(CS_APP_STATIC) $(OBJECTS)

# Create objects. Using $(OBJECTS) as a rule is shorthand for running this on all cpp files in $(SOURCES).
# $< refers to the first prerequisite, which is $(SOURCES). $@ refers to the target, which is $(OBJECTS)
$(CS_APP_OBJ_DIR)/%.o: $(CS_APP_SRC_ROOT)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@
$(CS_APP_OBJ_DIR)/%.o: $(CS_APP_SRC_ROOT)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@
$(CS_APP_OBJ_DIR)/%.o: $(CS_APP_SRC_ROOT)/%.cc
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_APP_OBJ_DIR)/' '--extensions' 'o')
	rm -f $(CS_APP_STATIC)
	rm -f $(CS_APP_EXECUTABLE)

cleanobjects:
	rm -f $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '(CS_APP_OBJ_DIR)/' '--extensions' 'o')
