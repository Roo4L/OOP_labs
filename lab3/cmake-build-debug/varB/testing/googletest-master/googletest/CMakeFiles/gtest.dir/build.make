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
include varB/testing/googletest-master/googletest/CMakeFiles/gtest.dir/depend.make

# Include the progress variables for this target.
include varB/testing/googletest-master/googletest/CMakeFiles/gtest.dir/progress.make

# Include the compile flags for this target's objects.
include varB/testing/googletest-master/googletest/CMakeFiles/gtest.dir/flags.make

varB/testing/googletest-master/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o: varB/testing/googletest-master/googletest/CMakeFiles/gtest.dir/flags.make
varB/testing/googletest-master/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o: ../varB/testing/googletest-master/googletest/src/gtest-all.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object varB/testing/googletest-master/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o"
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varB/testing/googletest-master/googletest && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gtest.dir/src/gtest-all.cc.o -c /home/copiedwonder/OOP_Labs/lab3/varB/testing/googletest-master/googletest/src/gtest-all.cc

varB/testing/googletest-master/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gtest.dir/src/gtest-all.cc.i"
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varB/testing/googletest-master/googletest && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/copiedwonder/OOP_Labs/lab3/varB/testing/googletest-master/googletest/src/gtest-all.cc > CMakeFiles/gtest.dir/src/gtest-all.cc.i

varB/testing/googletest-master/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gtest.dir/src/gtest-all.cc.s"
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varB/testing/googletest-master/googletest && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/copiedwonder/OOP_Labs/lab3/varB/testing/googletest-master/googletest/src/gtest-all.cc -o CMakeFiles/gtest.dir/src/gtest-all.cc.s

# Object files for target gtest
gtest_OBJECTS = \
"CMakeFiles/gtest.dir/src/gtest-all.cc.o"

# External object files for target gtest
gtest_EXTERNAL_OBJECTS =

lib/libgtestd.a: varB/testing/googletest-master/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o
lib/libgtestd.a: varB/testing/googletest-master/googletest/CMakeFiles/gtest.dir/build.make
lib/libgtestd.a: varB/testing/googletest-master/googletest/CMakeFiles/gtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../../../../lib/libgtestd.a"
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varB/testing/googletest-master/googletest && $(CMAKE_COMMAND) -P CMakeFiles/gtest.dir/cmake_clean_target.cmake
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varB/testing/googletest-master/googletest && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gtest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
varB/testing/googletest-master/googletest/CMakeFiles/gtest.dir/build: lib/libgtestd.a

.PHONY : varB/testing/googletest-master/googletest/CMakeFiles/gtest.dir/build

varB/testing/googletest-master/googletest/CMakeFiles/gtest.dir/clean:
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varB/testing/googletest-master/googletest && $(CMAKE_COMMAND) -P CMakeFiles/gtest.dir/cmake_clean.cmake
.PHONY : varB/testing/googletest-master/googletest/CMakeFiles/gtest.dir/clean

varB/testing/googletest-master/googletest/CMakeFiles/gtest.dir/depend:
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/copiedwonder/OOP_Labs/lab3 /home/copiedwonder/OOP_Labs/lab3/varB/testing/googletest-master/googletest /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varB/testing/googletest-master/googletest /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varB/testing/googletest-master/googletest/CMakeFiles/gtest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : varB/testing/googletest-master/googletest/CMakeFiles/gtest.dir/depend

