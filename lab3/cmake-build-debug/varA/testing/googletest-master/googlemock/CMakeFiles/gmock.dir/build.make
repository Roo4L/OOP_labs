# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

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
CMAKE_COMMAND = /snap/clion/129/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/129/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/copiedwonder/OOP_Labs/lab3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug

# Include any dependencies generated for this target.
include varA/testing/googletest-master/googlemock/CMakeFiles/gmock.dir/depend.make

# Include the progress variables for this target.
include varA/testing/googletest-master/googlemock/CMakeFiles/gmock.dir/progress.make

# Include the compile flags for this target's objects.
include varA/testing/googletest-master/googlemock/CMakeFiles/gmock.dir/flags.make

varA/testing/googletest-master/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o: varA/testing/googletest-master/googlemock/CMakeFiles/gmock.dir/flags.make
varA/testing/googletest-master/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o: ../varA/testing/googletest-master/googlemock/src/gmock-all.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object varA/testing/googletest-master/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o"
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varA/testing/googletest-master/googlemock && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gmock.dir/src/gmock-all.cc.o -c /home/copiedwonder/OOP_Labs/lab3/varA/testing/googletest-master/googlemock/src/gmock-all.cc

varA/testing/googletest-master/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gmock.dir/src/gmock-all.cc.i"
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varA/testing/googletest-master/googlemock && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/copiedwonder/OOP_Labs/lab3/varA/testing/googletest-master/googlemock/src/gmock-all.cc > CMakeFiles/gmock.dir/src/gmock-all.cc.i

varA/testing/googletest-master/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gmock.dir/src/gmock-all.cc.s"
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varA/testing/googletest-master/googlemock && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/copiedwonder/OOP_Labs/lab3/varA/testing/googletest-master/googlemock/src/gmock-all.cc -o CMakeFiles/gmock.dir/src/gmock-all.cc.s

# Object files for target gmock
gmock_OBJECTS = \
"CMakeFiles/gmock.dir/src/gmock-all.cc.o"

# External object files for target gmock
gmock_EXTERNAL_OBJECTS =

lib/libgmockd.a: varA/testing/googletest-master/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o
lib/libgmockd.a: varA/testing/googletest-master/googlemock/CMakeFiles/gmock.dir/build.make
lib/libgmockd.a: varA/testing/googletest-master/googlemock/CMakeFiles/gmock.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../../../../lib/libgmockd.a"
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varA/testing/googletest-master/googlemock && $(CMAKE_COMMAND) -P CMakeFiles/gmock.dir/cmake_clean_target.cmake
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varA/testing/googletest-master/googlemock && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gmock.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
varA/testing/googletest-master/googlemock/CMakeFiles/gmock.dir/build: lib/libgmockd.a

.PHONY : varA/testing/googletest-master/googlemock/CMakeFiles/gmock.dir/build

varA/testing/googletest-master/googlemock/CMakeFiles/gmock.dir/clean:
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varA/testing/googletest-master/googlemock && $(CMAKE_COMMAND) -P CMakeFiles/gmock.dir/cmake_clean.cmake
.PHONY : varA/testing/googletest-master/googlemock/CMakeFiles/gmock.dir/clean

varA/testing/googletest-master/googlemock/CMakeFiles/gmock.dir/depend:
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/copiedwonder/OOP_Labs/lab3 /home/copiedwonder/OOP_Labs/lab3/varA/testing/googletest-master/googlemock /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varA/testing/googletest-master/googlemock /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varA/testing/googletest-master/googlemock/CMakeFiles/gmock.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : varA/testing/googletest-master/googlemock/CMakeFiles/gmock.dir/depend

