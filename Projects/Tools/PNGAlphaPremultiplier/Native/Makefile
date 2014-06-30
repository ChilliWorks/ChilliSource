#-------------------------------------------------------------------
# Makefile
# Chilli Source
#
# Created by Ian Copland on 14/03/2012
# Copyright 2012 Tag Games. All rights reserved.
#-------------------------------------------------------------------
include Config.mk
#-------------------------------------------------------------------
# Definitions
#-------------------------------------------------------------------
CC				= gcc
CCXX			= g++
CFLAGS			= -std=c89 -m32 -Wall -O3 -DUSE_MMAP $(PROJECT_INCLUDE_PATHS)
CXXFLAGS		= -std=c++11 -m32 -Wall -O3 -DUSE_MMAP $(PROJECT_INCLUDE_PATHS)
LDFLAGS			= -m32
EXECUTABLE		= $(PROJECT_NAME)
CSOURCES		= $(PROJECT_C_SOURCE_FILES)
CXXSOURCES		= $(PROJECT_CPP_SOURCE_FILES)
COBJECTS		= $(CSOURCES:.c=.o)
CXXOBJECTS		= $(CXXSOURCES:.cpp=.o)
#-------------------------------------------------------------------
# All
#
# Builds everything. This is the default command.
#-------------------------------------------------------------------
all: $(EXECUTABLE)
#-------------------------------------------------------------------
# EXECUTABLE
#
# Builds the executable with the given name.
#-------------------------------------------------------------------
$(EXECUTABLE): $(CSOURCES) $(CXXSOURCES) $(COBJECTS) $(CXXOBJECTS)
	$(CCXX) $(LDFLAGS) $(COBJECTS) $(CXXOBJECTS) -o $@
#-------------------------------------------------------------------
# Clean
#
# Cleans everything.
#-------------------------------------------------------------------
clean:
	rm -rf $(COBJECTS) $(CXXOBJECTS)
#-------------------------------------------------------------------
# .c.o
#
# Specifies how all c object files are built
#-------------------------------------------------------------------
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
#-------------------------------------------------------------------
# .cpp.o
#
# Specifies how all cpp object files are built
#-------------------------------------------------------------------
.cpp.o:
	$(CCXX) $(CXXFLAGS) -c $< -o $@
