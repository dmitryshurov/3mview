# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dimax/Voreen

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dimax/Voreen

# Include any dependencies generated for this target.
include apps/simple/simple-qt/CMakeFiles/simple-qt.dir/depend.make

# Include the progress variables for this target.
include apps/simple/simple-qt/CMakeFiles/simple-qt.dir/progress.make

# Include the compile flags for this target's objects.
include apps/simple/simple-qt/CMakeFiles/simple-qt.dir/flags.make

apps/simple/simple-qt/CMakeFiles/simple-qt.dir/simple-qt.cpp.o: apps/simple/simple-qt/CMakeFiles/simple-qt.dir/flags.make
apps/simple/simple-qt/CMakeFiles/simple-qt.dir/simple-qt.cpp.o: apps/simple/simple-qt/simple-qt.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dimax/Voreen/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object apps/simple/simple-qt/CMakeFiles/simple-qt.dir/simple-qt.cpp.o"
	cd /home/dimax/Voreen/apps/simple/simple-qt && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/simple-qt.dir/simple-qt.cpp.o -c /home/dimax/Voreen/apps/simple/simple-qt/simple-qt.cpp

apps/simple/simple-qt/CMakeFiles/simple-qt.dir/simple-qt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simple-qt.dir/simple-qt.cpp.i"
	cd /home/dimax/Voreen/apps/simple/simple-qt && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/dimax/Voreen/apps/simple/simple-qt/simple-qt.cpp > CMakeFiles/simple-qt.dir/simple-qt.cpp.i

apps/simple/simple-qt/CMakeFiles/simple-qt.dir/simple-qt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simple-qt.dir/simple-qt.cpp.s"
	cd /home/dimax/Voreen/apps/simple/simple-qt && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/dimax/Voreen/apps/simple/simple-qt/simple-qt.cpp -o CMakeFiles/simple-qt.dir/simple-qt.cpp.s

apps/simple/simple-qt/CMakeFiles/simple-qt.dir/simple-qt.cpp.o.requires:
.PHONY : apps/simple/simple-qt/CMakeFiles/simple-qt.dir/simple-qt.cpp.o.requires

apps/simple/simple-qt/CMakeFiles/simple-qt.dir/simple-qt.cpp.o.provides: apps/simple/simple-qt/CMakeFiles/simple-qt.dir/simple-qt.cpp.o.requires
	$(MAKE) -f apps/simple/simple-qt/CMakeFiles/simple-qt.dir/build.make apps/simple/simple-qt/CMakeFiles/simple-qt.dir/simple-qt.cpp.o.provides.build
.PHONY : apps/simple/simple-qt/CMakeFiles/simple-qt.dir/simple-qt.cpp.o.provides

apps/simple/simple-qt/CMakeFiles/simple-qt.dir/simple-qt.cpp.o.provides.build: apps/simple/simple-qt/CMakeFiles/simple-qt.dir/simple-qt.cpp.o
.PHONY : apps/simple/simple-qt/CMakeFiles/simple-qt.dir/simple-qt.cpp.o.provides.build

# Object files for target simple-qt
simple__qt_OBJECTS = \
"CMakeFiles/simple-qt.dir/simple-qt.cpp.o"

# External object files for target simple-qt
simple__qt_EXTERNAL_OBJECTS =

bin/simple-qt: apps/simple/simple-qt/CMakeFiles/simple-qt.dir/simple-qt.cpp.o
bin/simple-qt: bin/libtgt.so
bin/simple-qt: bin/libvoreen_core.so
bin/simple-qt: bin/libvoreen_qt.so
bin/simple-qt: /usr/lib/libGLU.so
bin/simple-qt: /usr/lib/libGL.so
bin/simple-qt: /usr/lib/libX11.so
bin/simple-qt: /usr/lib/libGLEW.so
bin/simple-qt: /usr/local/lib/libboost_date_time.so
bin/simple-qt: /usr/local/lib/libboost_prg_exec_monitor.so
bin/simple-qt: /usr/local/lib/libboost_program_options.so
bin/simple-qt: /usr/local/lib/libboost_regex.so
bin/simple-qt: /usr/local/lib/libboost_thread.so
bin/simple-qt: /usr/local/lib/libboost_unit_test_framework.so
bin/simple-qt: /usr/lib/libIL.so
bin/simple-qt: /usr/lib/libILU.so
bin/simple-qt: /usr/lib/libz.so
bin/simple-qt: /usr/lib/libQtOpenGL.so
bin/simple-qt: /usr/lib/libQtGui.so
bin/simple-qt: /usr/lib/libpng.so
bin/simple-qt: /usr/lib/libfreetype.so
bin/simple-qt: /usr/lib/libX11.so
bin/simple-qt: /usr/lib/libm.so
bin/simple-qt: /usr/lib/libQtCore.so
bin/simple-qt: /usr/lib/libz.so
bin/simple-qt: /usr/lib/librt.so
bin/simple-qt: bin/libvoreen_core.so
bin/simple-qt: bin/libtgt.so
bin/simple-qt: /usr/lib/libGLU.so
bin/simple-qt: /usr/lib/libGL.so
bin/simple-qt: /usr/lib/libX11.so
bin/simple-qt: /usr/lib/libGLEW.so
bin/simple-qt: /usr/local/lib/libboost_date_time.so
bin/simple-qt: /usr/local/lib/libboost_prg_exec_monitor.so
bin/simple-qt: /usr/local/lib/libboost_program_options.so
bin/simple-qt: /usr/local/lib/libboost_regex.so
bin/simple-qt: /usr/local/lib/libboost_thread.so
bin/simple-qt: /usr/local/lib/libboost_unit_test_framework.so
bin/simple-qt: /usr/lib/libIL.so
bin/simple-qt: /usr/lib/libILU.so
bin/simple-qt: /usr/lib/libz.so
bin/simple-qt: apps/simple/simple-qt/CMakeFiles/simple-qt.dir/build.make
bin/simple-qt: apps/simple/simple-qt/CMakeFiles/simple-qt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../../bin/simple-qt"
	cd /home/dimax/Voreen/apps/simple/simple-qt && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simple-qt.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/simple/simple-qt/CMakeFiles/simple-qt.dir/build: bin/simple-qt
.PHONY : apps/simple/simple-qt/CMakeFiles/simple-qt.dir/build

apps/simple/simple-qt/CMakeFiles/simple-qt.dir/requires: apps/simple/simple-qt/CMakeFiles/simple-qt.dir/simple-qt.cpp.o.requires
.PHONY : apps/simple/simple-qt/CMakeFiles/simple-qt.dir/requires

apps/simple/simple-qt/CMakeFiles/simple-qt.dir/clean:
	cd /home/dimax/Voreen/apps/simple/simple-qt && $(CMAKE_COMMAND) -P CMakeFiles/simple-qt.dir/cmake_clean.cmake
.PHONY : apps/simple/simple-qt/CMakeFiles/simple-qt.dir/clean

apps/simple/simple-qt/CMakeFiles/simple-qt.dir/depend:
	cd /home/dimax/Voreen && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dimax/Voreen /home/dimax/Voreen/apps/simple/simple-qt /home/dimax/Voreen /home/dimax/Voreen/apps/simple/simple-qt /home/dimax/Voreen/apps/simple/simple-qt/CMakeFiles/simple-qt.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/simple/simple-qt/CMakeFiles/simple-qt.dir/depend

