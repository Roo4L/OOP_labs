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
include varB/testing/CMakeFiles/GoogleTests_run.dir/depend.make

# Include the progress variables for this target.
include varB/testing/CMakeFiles/GoogleTests_run.dir/progress.make

# Include the compile flags for this target's objects.
include varB/testing/CMakeFiles/GoogleTests_run.dir/flags.make

varB/testing/CMakeFiles/GoogleTests_run.dir/hex-test.cpp.o: varB/testing/CMakeFiles/GoogleTests_run.dir/flags.make
varB/testing/CMakeFiles/GoogleTests_run.dir/hex-test.cpp.o: ../varB/testing/hex-test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object varB/testing/CMakeFiles/GoogleTests_run.dir/hex-test.cpp.o"
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varB/testing && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/GoogleTests_run.dir/hex-test.cpp.o -c /home/copiedwonder/OOP_Labs/lab3/varB/testing/hex-test.cpp

varB/testing/CMakeFiles/GoogleTests_run.dir/hex-test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GoogleTests_run.dir/hex-test.cpp.i"
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varB/testing && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/copiedwonder/OOP_Labs/lab3/varB/testing/hex-test.cpp > CMakeFiles/GoogleTests_run.dir/hex-test.cpp.i

varB/testing/CMakeFiles/GoogleTests_run.dir/hex-test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GoogleTests_run.dir/hex-test.cpp.s"
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varB/testing && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/copiedwonder/OOP_Labs/lab3/varB/testing/hex-test.cpp -o CMakeFiles/GoogleTests_run.dir/hex-test.cpp.s

# Object files for target GoogleTests_run
GoogleTests_run_OBJECTS = \
"CMakeFiles/GoogleTests_run.dir/hex-test.cpp.o"

# External object files for target GoogleTests_run
GoogleTests_run_EXTERNAL_OBJECTS =

varB/testing/GoogleTests_run: varB/testing/CMakeFiles/GoogleTests_run.dir/hex-test.cpp.o
varB/testing/GoogleTests_run: varB/testing/CMakeFiles/GoogleTests_run.dir/build.make
varB/testing/GoogleTests_run: varB/source/libHex.a
varB/testing/GoogleTests_run: lib/libgtestd.a
varB/testing/GoogleTests_run: lib/libgtest_maind.a
varB/testing/GoogleTests_run: lib/libgtestd.a
varB/testing/GoogleTests_run: varB/testing/CMakeFiles/GoogleTests_run.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable GoogleTests_run"
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varB/testing && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GoogleTests_run.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
varB/testing/CMakeFiles/GoogleTests_run.dir/build: varB/testing/GoogleTests_run

.PHONY : varB/testing/CMakeFiles/GoogleTests_run.dir/build

varB/testing/CMakeFiles/GoogleTests_run.dir/clean:
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varB/testing && $(CMAKE_COMMAND) -P CMakeFiles/GoogleTests_run.dir/cmake_clean.cmake
.PHONY : varB/testing/CMakeFiles/GoogleTests_run.dir/clean

varB/testing/CMakeFiles/GoogleTests_run.dir/depend:
	cd /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/copiedwonder/OOP_Labs/lab3 /home/copiedwonder/OOP_Labs/lab3/varB/testing /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varB/testing /home/copiedwonder/OOP_Labs/lab3/cmake-build-debug/varB/testing/CMakeFiles/GoogleTests_run.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : varB/testing/CMakeFiles/GoogleTests_run.dir/depend
