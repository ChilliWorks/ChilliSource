#-------------------------------------------------------------------
# Config.mk
# Chilli Source
#
# Created by Ian Copland on 15/03/2012
# Copyright 2012 Tag Games. All rights reserved.
#-------------------------------------------------------------------
#-------------------------------------------------------------------
# PROJECT_NAME
#
# The name of the project. This will be used as the output
# exectable name.
#-------------------------------------------------------------------
PROJECT_NAME = PNGAlphaPremultiplier
#-------------------------------------------------------------------
# PROJECT_INCLUDE_PATHS
#
# A list of all the include paths used. This should be in the format
# -I<relativepath>
#-------------------------------------------------------------------
PROJECT_INCLUDE_PATHS = -ISource/ -ISource/LibPng/ -ISource/ZLib/
#-------------------------------------------------------------------
# PROJECT_C_SOURCE_FILES
#
# A list of all the c sources to be built in the project.
#-------------------------------------------------------------------
PROJECT_C_SOURCE_FILES = Source/PNGAlphaPremultiplier.c \
	Source/LibPng/png.c \
	Source/LibPng/pngerror.c \
	Source/LibPng/pngget.c \
	Source/LibPng/pngmem.c \
	Source/LibPng/pngpread.c \
	Source/LibPng/pngread.c \
	Source/LibPng/pngrio.c \
	Source/LibPng/pngrtran.c \
	Source/LibPng/pngrutil.c \
	Source/LibPng/pngset.c \
	Source/LibPng/pngtrans.c \
	Source/LibPng/pngwio.c \
	Source/LibPng/pngwrite.c \
	Source/LibPng/pngwtran.c \
	Source/LibPng/pngwutil.c \
	Source/ZLib/adler32.c \
	Source/ZLib/compress.c \
	Source/ZLib/crc32.c \
	Source/ZLib/deflate.c \
	Source/ZLib/gzclose.c \
	Source/ZLib/gzlib.c \
	Source/ZLib/gzread.c \
	Source/ZLib/gzwrite.c \
	Source/ZLib/infback.c \
	Source/ZLib/inffast.c \
	Source/ZLib/inflate.c \
	Source/ZLib/inftrees.c \
	Source/ZLib/trees.c \
	Source/ZLib/uncompr.c \
	Source/ZLib/zutil.c
#-------------------------------------------------------------------
# PROJECT_CPP_SOURCE_FILES
#
# A list of all the c++ sources to be built in the project.
#-------------------------------------------------------------------
PROJECT_CPP_SOURCE_FILES = Source/Main.cpp \
	Source/Logging.cpp \
	Source/CLogging.cpp \
	Source/StringUtils.cpp
