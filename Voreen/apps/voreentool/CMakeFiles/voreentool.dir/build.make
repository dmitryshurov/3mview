# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
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
CMAKE_SOURCE_DIR = /home/class3d/softw/Work/Voreen

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/class3d/softw/Work/Voreen

# Include any dependencies generated for this target.
include apps/voreentool/CMakeFiles/voreentool.dir/depend.make

# Include the progress variables for this target.
include apps/voreentool/CMakeFiles/voreentool.dir/progress.make

# Include the compile flags for this target's objects.
include apps/voreentool/CMakeFiles/voreentool.dir/flags.make

apps/voreentool/CMakeFiles/voreentool.dir/networkconfigurator.cpp.o: apps/voreentool/CMakeFiles/voreentool.dir/flags.make
apps/voreentool/CMakeFiles/voreentool.dir/networkconfigurator.cpp.o: apps/voreentool/networkconfigurator.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/class3d/softw/Work/Voreen/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object apps/voreentool/CMakeFiles/voreentool.dir/networkconfigurator.cpp.o"
	cd /home/class3d/softw/Work/Voreen/apps/voreentool && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/voreentool.dir/networkconfigurator.cpp.o -c /home/class3d/softw/Work/Voreen/apps/voreentool/networkconfigurator.cpp

apps/voreentool/CMakeFiles/voreentool.dir/networkconfigurator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/voreentool.dir/networkconfigurator.cpp.i"
	cd /home/class3d/softw/Work/Voreen/apps/voreentool && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/class3d/softw/Work/Voreen/apps/voreentool/networkconfigurator.cpp > CMakeFiles/voreentool.dir/networkconfigurator.cpp.i

apps/voreentool/CMakeFiles/voreentool.dir/networkconfigurator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/voreentool.dir/networkconfigurator.cpp.s"
	cd /home/class3d/softw/Work/Voreen/apps/voreentool && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/class3d/softw/Work/Voreen/apps/voreentool/networkconfigurator.cpp -o CMakeFiles/voreentool.dir/networkconfigurator.cpp.s

apps/voreentool/CMakeFiles/voreentool.dir/networkconfigurator.cpp.o.requires:
.PHONY : apps/voreentool/CMakeFiles/voreentool.dir/networkconfigurator.cpp.o.requires

apps/voreentool/CMakeFiles/voreentool.dir/networkconfigurator.cpp.o.provides: apps/voreentool/CMakeFiles/voreentool.dir/networkconfigurator.cpp.o.requires
	$(MAKE) -f apps/voreentool/CMakeFiles/voreentool.dir/build.make apps/voreentool/CMakeFiles/voreentool.dir/networkconfigurator.cpp.o.provides.build
.PHONY : apps/voreentool/CMakeFiles/voreentool.dir/networkconfigurator.cpp.o.provides

apps/voreentool/CMakeFiles/voreentool.dir/networkconfigurator.cpp.o.provides.build: apps/voreentool/CMakeFiles/voreentool.dir/networkconfigurator.cpp.o

apps/voreentool/CMakeFiles/voreentool.dir/voreentool.cpp.o: apps/voreentool/CMakeFiles/voreentool.dir/flags.make
apps/voreentool/CMakeFiles/voreentool.dir/voreentool.cpp.o: apps/voreentool/voreentool.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/class3d/softw/Work/Voreen/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object apps/voreentool/CMakeFiles/voreentool.dir/voreentool.cpp.o"
	cd /home/class3d/softw/Work/Voreen/apps/voreentool && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/voreentool.dir/voreentool.cpp.o -c /home/class3d/softw/Work/Voreen/apps/voreentool/voreentool.cpp

apps/voreentool/CMakeFiles/voreentool.dir/voreentool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/voreentool.dir/voreentool.cpp.i"
	cd /home/class3d/softw/Work/Voreen/apps/voreentool && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/class3d/softw/Work/Voreen/apps/voreentool/voreentool.cpp > CMakeFiles/voreentool.dir/voreentool.cpp.i

apps/voreentool/CMakeFiles/voreentool.dir/voreentool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/voreentool.dir/voreentool.cpp.s"
	cd /home/class3d/softw/Work/Voreen/apps/voreentool && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/class3d/softw/Work/Voreen/apps/voreentool/voreentool.cpp -o CMakeFiles/voreentool.dir/voreentool.cpp.s

apps/voreentool/CMakeFiles/voreentool.dir/voreentool.cpp.o.requires:
.PHONY : apps/voreentool/CMakeFiles/voreentool.dir/voreentool.cpp.o.requires

apps/voreentool/CMakeFiles/voreentool.dir/voreentool.cpp.o.provides: apps/voreentool/CMakeFiles/voreentool.dir/voreentool.cpp.o.requires
	$(MAKE) -f apps/voreentool/CMakeFiles/voreentool.dir/build.make apps/voreentool/CMakeFiles/voreentool.dir/voreentool.cpp.o.provides.build
.PHONY : apps/voreentool/CMakeFiles/voreentool.dir/voreentool.cpp.o.provides

apps/voreentool/CMakeFiles/voreentool.dir/voreentool.cpp.o.provides.build: apps/voreentool/CMakeFiles/voreentool.dir/voreentool.cpp.o

# Object files for target voreentool
voreentool_OBJECTS = \
"CMakeFiles/voreentool.dir/networkconfigurator.cpp.o" \
"CMakeFiles/voreentool.dir/voreentool.cpp.o"

# External object files for target voreentool
voreentool_EXTERNAL_OBJECTS =

bin/voreentool: apps/voreentool/CMakeFiles/voreentool.dir/networkconfigurator.cpp.o
bin/voreentool: apps/voreentool/CMakeFiles/voreentool.dir/voreentool.cpp.o
bin/voreentool: bin/libtgt.so
bin/voreentool: bin/libvoreen_core.so
bin/voreentool: bin/libvoreen_qt.so
bin/voreentool: /usr/lib/x86_64-linux-gnu/libGLU.so
bin/voreentool: /usr/lib/x86_64-linux-gnu/libGL.so
bin/voreentool: /usr/lib/x86_64-linux-gnu/libSM.so
bin/voreentool: /usr/lib/x86_64-linux-gnu/libICE.so
bin/voreentool: /usr/lib/x86_64-linux-gnu/libX11.so
bin/voreentool: /usr/lib/x86_64-linux-gnu/libXext.so
bin/voreentool: /usr/lib/x86_64-linux-gnu/libGLEW.so
bin/voreentool: /usr/lib/libboost_date_time-mt.so
bin/voreentool: /usr/lib/libboost_prg_exec_monitor-mt.so
bin/voreentool: /usr/lib/libboost_program_options-mt.so
bin/voreentool: /usr/lib/libboost_regex-mt.so
bin/voreentool: /usr/lib/libboost_thread-mt.so
bin/voreentool: /usr/lib/libboost_unit_test_framework-mt.so
bin/voreentool: /usr/lib/libIL.so
bin/voreentool: /usr/lib/libILU.so
bin/voreentool: /usr/lib/x86_64-linux-gnu/libz.so
bin/voreentool: /usr/lib/x86_64-linux-gnu/libQtOpenGL.so
bin/voreentool: /usr/lib/x86_64-linux-gnu/libQtGui.so
bin/voreentool: /usr/lib/x86_64-linux-gnu/libQtCore.so
bin/voreentool: bin/libvoreen_core.so
bin/voreentool: bin/libtgt.so
bin/voreentool: /usr/lib/x86_64-linux-gnu/libGLU.so
bin/voreentool: /usr/lib/x86_64-linux-gnu/libGL.so
bin/voreentool: /usr/lib/x86_64-linux-gnu/libSM.so
bin/voreentool: /usr/lib/x86_64-linux-gnu/libICE.so
bin/voreentool: /usr/lib/x86_64-linux-gnu/libX11.so
bin/voreentool: /usr/lib/x86_64-linux-gnu/libXext.so
bin/voreentool: /usr/lib/x86_64-linux-gnu/libGLEW.so
bin/voreentool: /usr/lib/libboost_date_time-mt.so
bin/voreentool: /usr/lib/libboost_prg_exec_monitor-mt.so
bin/voreentool: /usr/lib/libboost_program_options-mt.so
bin/voreentool: /usr/lib/libboost_regex-mt.so
bin/voreentool: /usr/lib/libboost_thread-mt.so
bin/voreentool: /usr/lib/libboost_unit_test_framework-mt.so
bin/voreentool: /usr/lib/libIL.so
bin/voreentool: /usr/lib/libILU.so
bin/voreentool: /usr/lib/x86_64-linux-gnu/libz.so
bin/voreentool: apps/voreentool/CMakeFiles/voreentool.dir/build.make
bin/voreentool: apps/voreentool/CMakeFiles/voreentool.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../bin/voreentool"
	cd /home/class3d/softw/Work/Voreen/apps/voreentool && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/voreentool.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/voreentool/CMakeFiles/voreentool.dir/build: bin/voreentool
.PHONY : apps/voreentool/CMakeFiles/voreentool.dir/build

apps/voreentool/CMakeFiles/voreentool.dir/requires: apps/voreentool/CMakeFiles/voreentool.dir/networkconfigurator.cpp.o.requires
apps/voreentool/CMakeFiles/voreentool.dir/requires: apps/voreentool/CMakeFiles/voreentool.dir/voreentool.cpp.o.requires
.PHONY : apps/voreentool/CMakeFiles/voreentool.dir/requires

apps/voreentool/CMakeFiles/voreentool.dir/clean:
	cd /home/class3d/softw/Work/Voreen/apps/voreentool && $(CMAKE_COMMAND) -P CMakeFiles/voreentool.dir/cmake_clean.cmake
.PHONY : apps/voreentool/CMakeFiles/voreentool.dir/clean

apps/voreentool/CMakeFiles/voreentool.dir/depend:
	cd /home/class3d/softw/Work/Voreen && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/class3d/softw/Work/Voreen /home/class3d/softw/Work/Voreen/apps/voreentool /home/class3d/softw/Work/Voreen /home/class3d/softw/Work/Voreen/apps/voreentool /home/class3d/softw/Work/Voreen/apps/voreentool/CMakeFiles/voreentool.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/voreentool/CMakeFiles/voreentool.dir/depend

