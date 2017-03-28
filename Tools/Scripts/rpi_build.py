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
import os
import subprocess
import sys
from ninja_syntax import Writer
from file_system_utils import delete_directory

COMPILER_FLAGS_DEFAULT = "-c -std=c++11 -fsigned-char -pthread -fexceptions -frtti -DCS_TARGETPLATFORM_RPI"
COMPILER_FLAGS_TARGET_MAP = { "debug":"-g -DDEBUG -DCS_ENABLE_DEBUG",  	"release":"-O3 -DNDEBUG" }
INCLUDE_PATHS = "-I{0}/ChilliSource/Libraries/Core/RPi/Headers -I{0}/ChilliSource/Source -I{0}/ChilliSource/Libraries/CricketAudio/RPi/Headers"
LIBRARIES = "-lChilliSource -lCSBase -lvcos -lbcm_host -lGLESv2 -lEGL -lvchiq_arm -lpthread -l:libX11.so.6.3.0 -l:libXau.so.6.0.0 -l:libXdmcp.so.6.0.0 -l:libxcb.so.1.1.0 -l:libxcb-xkb.so.1.0.0 -l:libxkbcommon.so.0.0.0 -l:libxkbcommon-x11.so.0.0.0"
LIBRARY_PATHS = "-L{0}/ChilliSource/Libraries/Core/RPi/Libs -L{0}/ChilliSource/Libraries/CricketAudio/RPi/Libs"

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
# @param project_root
# 	Path to the root of the project
# @param build_dir
# 	Location to output compiled files to
# 
# @return List of output file paths
# 
def _write_build_command(ninja_file, dirs, exts, compile_rule, project_root, build_dir):

	get_files_script = os.path.normpath("{}/ChilliSource/Tools/Scripts/get_file_paths_with_extensions.py".format(project_root))
	source_files = []
	for d in dirs:
		source_files += subprocess.check_output(['python', get_files_script, '--directory', d, '--extensions', exts], universal_newlines=True).split(" ")

	source_files = list(filter(lambda x: len(x) > 0, source_files))
	output_files = map(lambda x: os.path.splitext(x)[0]+'.o', source_files)
	output_files = list(map(lambda x: os.path.normpath(x).replace(project_root, build_dir, 1), output_files))

	for source_file, output_file in zip(source_files, output_files):
		ninja_file.build(rule=compile_rule, inputs=source_file, outputs=output_file)

	return output_files

# Generate a ninja "makefile"
#
# @param app_name
# 	Name of the executable to generate
# @param compiler_path
# 	Path to the gcc compiler
# @param linker_path
# 	Path to the gcc linker
# @param archiver_path
# 	Path to the gcc archiver for making static libs
# @param compiler_flags
#	Flags and defines for the compiler to use when compiling the source
# @param linker_flags
#	Flags for the linker to use when linking the exe
# @param app_source_dirs
#  	List of directories that have the app source files to compile
# @param project_root
# 	Root of the project e.g. the one containing ChilliSource, Content, AppSource, etc
# @param build_dir
# 	Location to output temp build files to
# @param output_dir
# 	Location to output executable and required assets
# @param lib_cs_path
#	path to temporary lib directory
# @param lib_app_path
#	path to temporary lib directory
#
def _generate_ninja_file(app_name,
	compiler_path, linker_path, archiver_path, 
	compiler_flags, linker_flags,
	app_source_dirs,
	project_root, build_dir, output_dir, lib_cs_path, lib_app_path):

	with open(os.path.join(build_dir, "Application.ninja"), "w") as build_file:
		ninja_file = Writer(build_file)

		ninja_file.variable(key="builddir", value=build_dir)

		# Write the compiler rule for c, cpp and cc
		ninja_file.rule("compile", command="{} -MMD -MT $out -MF $out.d {} $in -o $out".format(compiler_path, compiler_flags), description="Compiling source: $in", depfile="$out.d", deps="gcc")

		# Write the rule to build the static library. Note we use the @file notation as on Windows the command is too long for CreateProcess
		ninja_file.rule("ar", command="{} rcs $out @$in".format(archiver_path), description="Building static library: $out")

		# Write the rule to link. Note we use the @file notation as on Windows the command is too long for CreateProcess
		ninja_file.rule("ld", command="{} @$in {} -o $out".format(linker_path, linker_flags), description="Linking: $out")

		# Write the compile command for all source files.
		cs_source_dirs = [os.path.normpath('{}/ChilliSource/Source/ChilliSource'.format(project_root)), os.path.normpath('{}/ChilliSource/Source/CSBackend/Platform/RPi/'.format(project_root)), os.path.normpath('{}/ChilliSource/Source/CSBackend/Rendering/OpenGL/'.format(project_root))]
		cs_output_files = _write_build_command(ninja_file, cs_source_dirs, 'c,cpp,cc', 'compile', project_root, build_dir)
		app_output_files = _write_build_command(ninja_file, app_source_dirs, 'c,cpp,cc', 'compile', project_root, build_dir)

		# Create the response file to get around exceeding the command length on Windows.
		joint_output_filepath = os.path.join(build_dir, 'jointoutputfiles.rsp')
		joint_output_file = open(joint_output_filepath, 'w') 

		cs_output_filepath = os.path.join(build_dir, 'csoutputfiles.rsp')
		cs_output_file = open(cs_output_filepath, 'w') 

		app_output_filepath = os.path.join(build_dir, 'appoutputfiles.rsp')
		app_output_file = open(app_output_filepath, 'w') 

		for o in cs_output_files:
			escaped = o.replace('\\', '\\\\')
			cs_output_file.write("{} ".format(escaped))
			joint_output_file.write("{} ".format(escaped))

		for o in app_output_files:
			escaped = o.replace('\\', '\\\\')
			app_output_file.write("{} ".format(escaped))
			joint_output_file.write("{} ".format(escaped))

		joint_output_file.close()
		cs_output_file.close()
		app_output_file.close()

		# Write the command to generate the static library for ChilliSource and the application
		ninja_file.build(rule="ar", inputs=cs_output_filepath, outputs=lib_cs_path)
		ninja_file.build(rule="ar", inputs=app_output_filepath, outputs=lib_app_path)

		# Write the rule to link the libraries into the executable
		ninja_file.build(rule="ld", inputs=joint_output_filepath, outputs=os.path.join(output_dir, app_name))

# Generates the ninja "makefile", builds the application and copies
# the assets to the output folder
#
# @param target_scheme
# 	Used to apply compiler flags
# @param num_jobs
# 	Used to restrict the number of concurrent build jobs. If None then unrestricted
# @param app_name
# 	Name of the executable to generate
# @param compiler_path
# 	Path to the gcc compiler
# @param linker_path
# 	Path to the gcc linker
# @param archiver_path
# 	Path to the gcc archiver for making static libs
# @param additional_libs
# 	Any additional libraries in the format "-lLib1 -lLib2"
# @param additional_lib_paths
# 	Any additional libraries directories in the format "-LLib/Path1 -LLib/Path2"
# @param additional_include_paths
# 	Any additional header file directories in the format "-IInclude/Path1 -IInclude/Path2"
# @param additional_compiler_flags_map
# 	Dict containing compiler flags for debug/release in the format -DDEBUG -g"
# @param app_source_dirs
#  	List of directories that have the app source files to compile
# @param project_root
# 	Root of the project e.g. the one containing ChilliSource, Content, AppSource, etc
# @param build_dir
# 	Location to output temp build files to
# @param output_dir
# 	Location to output executable and required assets
# @param lib_cs_path
#	path to temporary lib directory
# @param lib_app_path
#	path to temporary lib directory
#
def _build(target_scheme,
	num_jobs,
	app_name,
	compiler_path, linker_path, archiver_path, 
	additional_libs, additional_lib_paths, additional_include_paths,
	additional_compiler_flags_map,
	app_source_dirs,
	project_root, build_dir, output_dir, lib_cs_path, lib_app_path):

	try:
		os.makedirs(build_dir)
	except OSError:
		print("Build directory already exists")

	try:
		os.makedirs(output_dir)
	except OSError:
		print("Output directory already exists")

	compiler_flags = COMPILER_FLAGS_DEFAULT + " " + COMPILER_FLAGS_TARGET_MAP[target_scheme] + " " + additional_compiler_flags_map[target_scheme] + " " + additional_include_paths + " " + INCLUDE_PATHS.format(project_root)
	linker_flags = additional_lib_paths + " " + LIBRARY_PATHS.format(project_root) + " -L" + build_dir + " " + additional_libs + " " + "-l{}".format(app_name) + " " + LIBRARIES

	# Remove the old libraries and exes but not the compiled files
	_clean(None, output_dir, lib_cs_path, lib_app_path)

	# Generate the ninja "makefile" based on the target scheme and platform
	_generate_ninja_file(app_name, compiler_path, linker_path, archiver_path, compiler_flags, linker_flags, app_source_dirs, project_root, build_dir, output_dir, lib_cs_path, lib_app_path)

	# Build the exe using ninja
	if num_jobs == None:
		subprocess.call(['ninja', '-f', os.path.join(build_dir, 'Application.ninja')])
	else:
		subprocess.call(['ninja', '-f', os.path.join(build_dir, 'Application.ninja'), '-j', str(num_jobs)])
	
	# Copy the assets ready for packaging
	copy_resources_script = os.path.normpath('{}/ChilliSource/Tools/Scripts/copy_rpi_resources.py'.format(project_root))
	subprocess.call(['python', copy_resources_script, project_root, output_dir])

# Cleans the output, build and libs. Passing None
# will prevent them from being cleaned
#
# @param build_dir
#	Location for temporary build files
# @param output_dir
#	Location for output exe and assets
# @param lib_cs_path
#	path to temporary lib directory
# @param lib_app_path
#	path to temporary lib directory
#
def _clean(build_dir, output_dir, lib_cs_path, lib_app_path):

	if lib_cs_path != None and os.path.isfile(lib_cs_path):
		os.remove(lib_cs_path)
	if lib_app_path != None and os.path.isfile(lib_app_path):
		os.remove(lib_app_path)
	if build_dir != None and os.path.isdir(build_dir):
		delete_directory(build_dir)
	if output_dir != None and os.path.isdir(output_dir):
		delete_directory(output_dir)

# Begin building for Raspberry Pi
#
# @param args 
#	The list of arguments - Should have an additional argument "debug" or "release" optionally followed by "clean"
# @param num_jobs
# 	Used to restrict the number of concurrent build jobs. If None then unrestricted
# @param app_name
# 	Name of the executable to generate
# @param compiler_path
# 	Path to the gcc compiler
# @param linker_path
# 	Path to the gcc linker
# @param archiver_path
# 	Path to the gcc archiver for making static libs
# @param additional_libs
# 	Any additional libraries in the format "-lLib1 -lLib2"
# @param additional_lib_paths
# 	Any additional libraries directories in the format "-LLib/Path1 -LLib/Path2"
# @param additional_include_paths
# 	Any additional header file directories in the format "-IInclude/Path1 -IInclude/Path2"
# @param additional_compiler_flags_map
# 	Dict containing compiler flags for debug/release in the format -DDEBUG -g"
# @param app_source_dirs
#  	List of directories that have the app source files to compile
# @param project_root
# 	Root of the project e.g. the one containing ChilliSource, Content, AppSource, etc
# @param build_root
# 	Location to output temp build files to
# @param output_root
# 	Location to output executable and required assets
#
def run(args,
	num_jobs,
	app_name,
	compiler_path, linker_path, archiver_path, 
	additional_libs, additional_lib_paths, additional_include_paths,
	additional_compiler_flags_map,
	app_source_dirs,
	project_root, build_root, output_root):

	if len(args) < 1:
		print("Too few args. Usage: debug|release [clean]")
		return

	target_scheme = args[0].lower()

	if target_scheme not in ['debug', 'release']:
		print("Incorrect target scheme {}. Usage: debug|release [clean]".format(target_scheme))
		return

	build_dir = os.path.normpath("{}/{}".format(build_root, target_scheme))
	output_dir = os.path.normpath("{}/{}".format(output_root, target_scheme))
	lib_cs_path = os.path.normpath("{}/libChilliSource.a".format(build_dir))
	lib_app_path = os.path.normpath("{}/lib{}.a".format(build_dir, app_name))

	if len(args) > 1 and args[1].lower() == "clean":
		# Remove all libraries, exes and compiled files
		_clean(build_dir, output_dir, lib_cs_path, lib_app_path)
	else:
		_build(target_scheme, num_jobs, app_name, compiler_path, linker_path, archiver_path, additional_libs, additional_lib_paths, additional_include_paths, additional_compiler_flags_map, app_source_dirs, project_root, build_dir, output_dir, lib_cs_path, lib_app_path)
