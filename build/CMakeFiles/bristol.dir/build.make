# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.15.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.15.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/hzk/Downloads/uc_work

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/hzk/Downloads/uc_work/build

# Include any dependencies generated for this target.
include CMakeFiles/bristol.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bristol.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bristol.dir/flags.make

CMakeFiles/bristol.dir/src/bristol_to_SHDL.cpp.o: CMakeFiles/bristol.dir/flags.make
CMakeFiles/bristol.dir/src/bristol_to_SHDL.cpp.o: ../src/bristol_to_SHDL.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hzk/Downloads/uc_work/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/bristol.dir/src/bristol_to_SHDL.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bristol.dir/src/bristol_to_SHDL.cpp.o -c /Users/hzk/Downloads/uc_work/src/bristol_to_SHDL.cpp

CMakeFiles/bristol.dir/src/bristol_to_SHDL.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bristol.dir/src/bristol_to_SHDL.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/hzk/Downloads/uc_work/src/bristol_to_SHDL.cpp > CMakeFiles/bristol.dir/src/bristol_to_SHDL.cpp.i

CMakeFiles/bristol.dir/src/bristol_to_SHDL.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bristol.dir/src/bristol_to_SHDL.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/hzk/Downloads/uc_work/src/bristol_to_SHDL.cpp -o CMakeFiles/bristol.dir/src/bristol_to_SHDL.cpp.s

# Object files for target bristol
bristol_OBJECTS = \
"CMakeFiles/bristol.dir/src/bristol_to_SHDL.cpp.o"

# External object files for target bristol
bristol_EXTERNAL_OBJECTS =

bristol: CMakeFiles/bristol.dir/src/bristol_to_SHDL.cpp.o
bristol: CMakeFiles/bristol.dir/build.make
bristol: CMakeFiles/bristol.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/hzk/Downloads/uc_work/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bristol"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bristol.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bristol.dir/build: bristol

.PHONY : CMakeFiles/bristol.dir/build

CMakeFiles/bristol.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bristol.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bristol.dir/clean

CMakeFiles/bristol.dir/depend:
	cd /Users/hzk/Downloads/uc_work/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hzk/Downloads/uc_work /Users/hzk/Downloads/uc_work /Users/hzk/Downloads/uc_work/build /Users/hzk/Downloads/uc_work/build /Users/hzk/Downloads/uc_work/build/CMakeFiles/bristol.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bristol.dir/depend

