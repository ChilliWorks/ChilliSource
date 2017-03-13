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
CS_SRC_ROOT=$(CS_PROJECT_ROOT)/ChilliSource/Source

# Compiler Flags
CS_CXXFLAGS_TARGET_DEBUG = -g -DDEBUG -DCS_ENABLE_DEBUG -DCS_LOGLEVEL_VERBOSE -DCS_TARGETPLATFORM_RPI $(CS_INCLUDES)
CS_CXXFLAGS_TARGET_RELEASE = -O3 -DNDEBUG -DCS_LOGLEVEL_WARNING -DCS_TARGETPLATFORM_RPI $(CS_INCLUDES)

# Additional Compiler Flags
CFLAGS=-c -std=c++11 -fsigned-char -pthread -fexceptions -frtti

# Conditional DEBUG/RELEASE behaviour
DEBUG ?= 0
ifeq ($(DEBUG), 1)
	BUILD_OUTPUT_PATH=$(CS_PROJECT_ROOT)/Projects/RPi/Build/debug
	CFLAGS += $(CS_CXXFLAGS_TARGET_DEBUG)
else
	BUILD_OUTPUT_PATH=$(CS_PROJECT_ROOT)/Projects/RPi/Build/release
	CFLAGS += $(CS_CXXFLAGS_TARGET_RELEASE)
endif

# Set up scripts.
CS_SCRIPT_GETFILESWITHEXTENSIONS = $(CS_PROJECT_ROOT)/ChilliSource/Tools/Scripts/get_file_paths_with_extensions.py

# Gather all files in the engine that should be built.
CS_SOURCEFILES_CHILLISOURCE := $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_SRC_ROOT)/ChilliSource/' '--extensions' 'cpp,c,cc')
CS_SOURCEFILES_PLATFORM := $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_SRC_ROOT)/CSBackend/Platform/RPi/' '--extensions' 'cpp,c,cc')
CS_SOURCEFILES_RENDERING := $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_SRC_ROOT)/CSBackend/Rendering/OpenGL/' '--extensions' 'cpp,c,cc')

# Set up tools.
CC=/Volumes/xtools/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-g++
AR=/Volumes/xtools/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-ar

# Includes
CS_INCLUDES=-I$(CS_PROJECT_ROOT)/ChilliSource/Libraries/Core/RPi/Headers -I$(CS_PROJECT_ROOT)/ChilliSource/Source -I$(CS_PROJECT_ROOT)/ChilliSource/Libraries/CricketAudio/RPi/Headers

# Library Directories
CS_LIBRARY_DIRS=-L$(CS_PROJECT_ROOT)/ChilliSource/Libraries/Core/RPi/Libs -L$(CS_PROJECT_ROOT)/ChilliSource/Libraries/CricketAudio/RPi/Libs

# All Source Files
SOURCES=$(CS_SOURCEFILES_CHILLISOURCE) $(CS_SOURCEFILES_PLATFORM) $(CS_SOURCEFILES_RENDERING)

# All Objects to be Generated - they take their names from the names of the cpp files that generated them.
CS_OBJ_DIR = $(BUILD_OUTPUT_PATH)/csobj
OBJECTS := $(patsubst $(CS_SRC_ROOT)/%.cpp, $(CS_OBJ_DIR)/%.o, $(filter %.cpp, $(SOURCES))) $(patsubst $(CS_SRC_ROOT)/%.cc, $(CS_OBJ_DIR)/%.o, $(filter %.cc, $(SOURCES))) $(patsubst $(CS_SRC_ROOT)/%.c, $(CS_OBJ_DIR)/%.o, $(filter %.c, $(SOURCES)))

# Name of static lib to generate.
CS_STATIC_LIB=$(BUILD_OUTPUT_PATH)/libChilliSource.a

.PHONY: clean cleanobjects cleangenerated

# Default make command.
all: $(SOURCES) $(CS_STATIC_LIB)

# Link objs into static lib. Uses the .cpp.o: rule below.
$(CS_STATIC_LIB): $(OBJECTS)
	$(AR) rcs $(CS_STATIC_LIB) $(OBJECTS)

# Create objects. Using $(OBJECTS) as a rule is shorthand for running this on all cpp files in $(SOURCES).
# $< refers to the first prerequisite, which is $(SOURCES). $@ refers to the target, which is $(OBJECTS)
$(CS_OBJ_DIR)/%.o: $(CS_SRC_ROOT)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@
$(CS_OBJ_DIR)/%.o: $(CS_SRC_ROOT)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@
$(CS_OBJ_DIR)/%.o: $(CS_SRC_ROOT)/%.cc
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_OBJ_DIR)/' '--extensions' 'o')
	rm -f $(CS_STATIC_LIB)

cleanobjects:
	rm -f $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_OBJ_DIR)/' '--extensions' 'o')

cleangenerated:
	rm -f $(CS_STATIC_LIB)
