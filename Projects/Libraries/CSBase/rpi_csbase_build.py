#!/usr/bin/python
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
import sys
CS_ROOT = "../../.."
sys.path.append("{}/Tools/Scripts/".format(CS_ROOT))
import subprocess
import os
from ninja_syntax import Writer
from file_system_utils import delete_directory

COMPILER_FLAGS_DEFAULT_CPP = "-c -std=c++11 -fsigned-char -pthread -fexceptions -frtti -DCS_TARGETPLATFORM_RPI"
COMPILER_FLAGS_DEFAULT_C = "-c -fsigned-char -pthread -fexceptions -DCS_TARGETPLATFORM_RPI"
COMPILER_FLAGS_TARGET_MAP = { "debug":"-g -DDEBUG -DCS_ENABLE_DEBUG",  	"release":"-O3 -DNDEBUG" }
SOURCE_PATHS = ["Source"]
INCLUDE_PATHS = "-IPlatformSource/RPi -I{}".format(" ".join(SOURCE_PATHS))

# Write build commands to the given ninja file for all source files that have the given extension in the given directories
#
# @param ninja_file
#	File to write commands to
# @param dirs
#	List of directories to search for source files
# @param exts
# 	The list source file extension to look for in the format "a,b,c"
# @param compile_rule
# 	Name of the compile rule to compile this type of source file
# @param dep_rule
# 	Name of the rule to generate depencencies for this type of source file
# @param build_dir
# 	Location to output compiled files to
# 
# @return List of output file paths
# 
def _write_build_command(ninja_file, dirs, exts, compile_rule, dep_rule, build_dir):

	get_files_script = os.path.normpath("{}/Tools/Scripts/get_file_paths_with_extensions.py".format(CS_ROOT))
	source_files = []
	for d in dirs:
		source_files += subprocess.check_output(['python', get_files_script, '--directory', d, '--extensions', exts], universal_newlines=True).split(" ")

	source_files = list(filter(lambda x: len(x) > 0, source_files))
	source_files = list(filter(lambda x: len(x) > 0, source_files))
	# Convert the source files extensions from .c/cpp etc. to .o
	output_files = map(lambda x: os.path.splitext(x)[0]+'.o', source_files)
	# Make the file paths relative to the build dir
	output_files = map(lambda x: os.path.join(build_dir, x), output_files)
	# Convert from windows separators to unix ones as Ninja has a bug where it doesn't escape windows properly
	output_files = list(map(lambda x: x.replace('\\', '/'), output_files))

	for source_file, output_file in zip(source_files, output_files):
		ninja_file.build(rule=compile_rule, inputs=source_file, outputs=output_file)
		ninja_file.build(rule=dep_rule, inputs=source_file, outputs=output_file+".d")

	return output_files

# Generate a ninja "makefile"
#
# @param cpp_compiler_path
# 	Path to the g++ compiler
# @param c_compiler_path
# 	Path to the gcc compiler
# @param archiver_path
# 	Path to the gcc archiver for making static libs
# @param target_scheme
# 	Used to apply compiler flags
# @param build_dir
# 	Location to output temp build files to
# @param lib_path
#	path to temporary lib directory
#
def _generate_ninja_file(cpp_compiler_path, c_compiler_path, archiver_path, target_scheme, build_dir, lib_path):

	with open(os.path.join(build_dir, "Application.ninja"), "w") as build_file:
		ninja_file = Writer(build_file)

		ninja_file.variable(key="builddir", value=build_dir)

		cpp_compiler_flags = COMPILER_FLAGS_DEFAULT_CPP + " " + COMPILER_FLAGS_TARGET_MAP[target_scheme] + " " + INCLUDE_PATHS
		c_compiler_flags = COMPILER_FLAGS_DEFAULT_C + " " + COMPILER_FLAGS_TARGET_MAP[target_scheme] + " " + INCLUDE_PATHS

		# Write the compiler rule for c, cpp and cc
		ninja_file.rule("compile_cpp", command="{} {} -o $out $in".format(cpp_compiler_path, cpp_compiler_flags), description="Compiling C++ source: $in", depfile="$out.o.d", deps="gcc")
		ninja_file.rule("compile_c", command="{} {} -o $out $in".format(c_compiler_path, c_compiler_flags), description="Compiling C source: $in", depfile="$out.o.d", deps="gcc")

		# Write the rule that generates the dependencies
		ninja_file.rule("dependencies_cpp", command="{} {} -MM -MG -MF $out $in".format(cpp_compiler_path, cpp_compiler_flags), description="Generating C++ dependency: $in")
		ninja_file.rule("dependencies_c", command="{} {} -MM -MG -MF $out $in".format(c_compiler_path, c_compiler_flags), description="Generating C dependency: $in")

		# Write the rule to build the static library. Note we use response files as on Windows the command is too long for CreateProcess
		ninja_file.rule("archive", command="{} rcs $out @$out.rsp".format(archiver_path), description="Building static library: $out", rspfile="$out.rsp", rspfile_content="$in")

		# Write the compile command for all source files.
		output_files = _write_build_command(ninja_file, SOURCE_PATHS, 'cpp', 'compile_cpp', 'dependencies_cpp', build_dir)
		output_files += _write_build_command(ninja_file, SOURCE_PATHS, 'c,cc', 'compile_c', 'dependencies_c', build_dir)

		# Write the command to generate the static library for ChilliSource and the application
		ninja_file.build(rule="archive", inputs=output_files, outputs=lib_path)

# Generates the ninja "makefile", builds the application and copies
# the assets to the output folder
#
# @param target_scheme
# 	Used to apply compiler flags
# @param num_jobs
# 	Used to restrict the number of concurrent build jobs. If None then unrestricted
# @param cpp_compiler_path
# 	Path to the g++ compiler
# @param c_compiler_path
# 	Path to the gcc compiler
# @param archiver_path
# 	Path to the gcc archiver for making static libs
# @param build_dir
# 	Location to output temp build files to
# @param lib_path
#	path to temporary lib directory
#
def _build(target_scheme, num_jobs, cpp_compiler_path, c_compiler_path, archiver_path, build_dir, lib_path):

	try:
		os.makedirs(build_dir)
	except OSError:
		print("Build directory already exists")

	# Remove the old libraries and exes but not the compiled files
	_clean(None, lib_path)

	# Generate the ninja "makefile" based on the target scheme and platform
	_generate_ninja_file(cpp_compiler_path, c_compiler_path, archiver_path, target_scheme, build_dir, lib_path)

	# Build the exe using ninja
	if num_jobs == "None":
		subprocess.call(['ninja', '-f', os.path.join(build_dir, 'Application.ninja')])
	else:
		subprocess.call(['ninja', '-f', os.path.join(build_dir, 'Application.ninja'), '-j', str(num_jobs)])

# Cleans the output, build and libs. Passing None
# will prevent them from being cleaned
#
# @param build_dir
#	Location for temporary build files
# @param lib_path
#	path to temporary lib directory
#
def _clean(build_dir, lib_path):

	if lib_path != None and os.path.isfile(lib_path):
		os.remove(lib_path)
	if build_dir != None and os.path.isdir(build_dir):
		delete_directory(build_dir)

# Begin building for Raspberry Pi
#
# @param args 
#	The list of arguments - Should have an additional argument "debug" or "release" optionally followed by "clean" then numjobs, c++ compiler path, c compiler path and archiver path e.g "debug clean 2 g++ gcc ar"
#
def run(args):

	if len(args) < 4 and len(args) != 2:
		print("Too few args {}. Usage: debug|release [clean] <numjobs> <cpp_compiler> <c_compiler> <archiver>".format(len(args)))
		return

	target_scheme = args[0].lower()

	if target_scheme not in ['debug', 'release']:
		print("Incorrect target scheme {}. Usage: debug|release [clean] <numjobs> <cpp_compiler> <c_compiler> <archiver>".format(target_scheme))
		return

	build_dir = os.path.normpath("build/RPi/{}".format(target_scheme))
	lib_path = os.path.normpath("{}/libCSBase.a".format(build_dir))

	if len(args) > 1 and args[1].lower() == "clean":
		# Remove all libraries, exes and compiled files
		_clean(build_dir, lib_path)
	else:
		_build(target_scheme, args[1], args[2], args[3], args[4], build_dir, lib_path)

if __name__ == "__main__":
	run(sys.argv[1:])