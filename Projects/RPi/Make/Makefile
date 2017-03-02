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

# Set up scripts.
CS_SCRIPT_GETFILESWITHEXTENSIONS = $(CS_PROJECT_ROOT)/ChilliSource/Tools/Scripts/get_file_paths_with_extensions.py

# Gather all files in the engine that should be built.
CS_SOURCEFILES_CHILLISOURCE := $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_PROJECT_ROOT)/ChilliSource/Source/ChilliSource/' '--extensions' 'cpp,c,cc')
CS_SOURCEFILES_PLATFORM := $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_PROJECT_ROOT)/ChilliSource/Source/CSBackend/Platform/RPi/' '--extensions' 'cpp,c,cc')
CS_SOURCEFILES_RENDERING := $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_PROJECT_ROOT)/ChilliSource/Source/CSBackend/Rendering/OpenGL/' '--extensions' 'cpp,c,cc')

# Set up tools.
CC=/Volumes/xtools/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-g++
AR=/Volumes/xtools/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-ar

# Compiler Flags
CS_CXXFLAGS_TARGET=-DDEBUG -DCS_ENABLE_DEBUG -DCS_LOGLEVEL_VERBOSE -DCS_TARGETPLATFORM_RPI -DCS_OPENGLVERSION_ES

# Includes
CS_INCLUDES=-I$(CS_PROJECT_ROOT)/ChilliSource/Libraries/Core/RPi/Headers -I$(CS_PROJECT_ROOT)/ChilliSource/Source -I$(CS_PROJECT_ROOT)/ChilliSource/Libraries/CricketAudio/RPi/Headers

# Additional Compiler Flags
CFLAGS=-c -O3 -std=c++11 $(CS_CXXFLAGS_TARGET) $(CS_INCLUDES)

# Library Directories
CS_LIBRARY_DIRS=-L$(CS_PROJECT_ROOT)/ChilliSource/Libraries/Core/RPi/Libs -L$(CS_PROJECT_ROOT)/ChilliSource/Libraries/CricketAudio/RPi/Libs

# Linker Flags
LDFLAGS= $(CS_LIBRARY_DIRS) -lvcos -lbcm_host -lGLESv2 -lEGL -lvchiq_arm

# All Source Files
SOURCES=$(CS_SOURCEFILES_CHILLISOURCE) $(CS_SOURCEFILES_PLATFORM) $(CS_SOURCEFILES_RENDERING)

# All Objects to be Generated
OBJECTS=$(SOURCES:%.cpp=%.o) $(SOURCES:%.c=%.o) $(SOURCES:$.cc=$.o)

# Name of static lib to generate.
CS_STATIC_LIB=libChilliSource.a

# Name of application to generate.
EXECUTABLE=Application

# Default make command.
all: $(SOURCES) $(CS_STATIC_LIB)

# Link objs into static lib.
$(CS_STATIC_LIB): $(OBJECTS)
	$(AR) rcs $(CS_STATIC_LIB) $(OBJECTS)

# Create executable
executable: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# Create objects only.
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_PROJECT_ROOT)/ChilliSource/Source/ChilliSource/' '--extensions' 'o')
	rm $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_PROJECT_ROOT)/ChilliSource/Source/CSBackend/Platform/RPi/' '--extensions' 'o')
	rm $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_PROJECT_ROOT)/ChilliSource/Source/CSBackend/Rendering/OpenGL/' '--extensions' 'o')
	rm $(CS_STATIC_LIB)

cleanobjects:
	rm $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_PROJECT_ROOT)/ChilliSource/Source/ChilliSource/' '--extensions' 'o')
	rm $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_PROJECT_ROOT)/ChilliSource/Source/CSBackend/Platform/RPi/' '--extensions' 'o')
	rm $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_PROJECT_ROOT)/ChilliSource/Source/CSBackend/Rendering/OpenGL/' '--extensions' 'o')


