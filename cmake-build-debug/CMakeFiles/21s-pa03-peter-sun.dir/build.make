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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/peter/Algo/21s-pa03-peter-sun

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/peter/Algo/21s-pa03-peter-sun/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/21s-pa03-peter-sun.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/21s-pa03-peter-sun.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/21s-pa03-peter-sun.dir/flags.make

CMakeFiles/21s-pa03-peter-sun.dir/src/main.cpp.o: CMakeFiles/21s-pa03-peter-sun.dir/flags.make
CMakeFiles/21s-pa03-peter-sun.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/peter/Algo/21s-pa03-peter-sun/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/21s-pa03-peter-sun.dir/src/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/21s-pa03-peter-sun.dir/src/main.cpp.o -c /Users/peter/Algo/21s-pa03-peter-sun/src/main.cpp

CMakeFiles/21s-pa03-peter-sun.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/21s-pa03-peter-sun.dir/src/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/peter/Algo/21s-pa03-peter-sun/src/main.cpp > CMakeFiles/21s-pa03-peter-sun.dir/src/main.cpp.i

CMakeFiles/21s-pa03-peter-sun.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/21s-pa03-peter-sun.dir/src/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/peter/Algo/21s-pa03-peter-sun/src/main.cpp -o CMakeFiles/21s-pa03-peter-sun.dir/src/main.cpp.s

# Object files for target 21s-pa03-peter-sun
21s__pa03__peter__sun_OBJECTS = \
"CMakeFiles/21s-pa03-peter-sun.dir/src/main.cpp.o"

# External object files for target 21s-pa03-peter-sun
21s__pa03__peter__sun_EXTERNAL_OBJECTS =

21s-pa03-peter-sun: CMakeFiles/21s-pa03-peter-sun.dir/src/main.cpp.o
21s-pa03-peter-sun: CMakeFiles/21s-pa03-peter-sun.dir/build.make
21s-pa03-peter-sun: CMakeFiles/21s-pa03-peter-sun.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/peter/Algo/21s-pa03-peter-sun/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable 21s-pa03-peter-sun"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/21s-pa03-peter-sun.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/21s-pa03-peter-sun.dir/build: 21s-pa03-peter-sun

.PHONY : CMakeFiles/21s-pa03-peter-sun.dir/build

CMakeFiles/21s-pa03-peter-sun.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/21s-pa03-peter-sun.dir/cmake_clean.cmake
.PHONY : CMakeFiles/21s-pa03-peter-sun.dir/clean

CMakeFiles/21s-pa03-peter-sun.dir/depend:
	cd /Users/peter/Algo/21s-pa03-peter-sun/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/peter/Algo/21s-pa03-peter-sun /Users/peter/Algo/21s-pa03-peter-sun /Users/peter/Algo/21s-pa03-peter-sun/cmake-build-debug /Users/peter/Algo/21s-pa03-peter-sun/cmake-build-debug /Users/peter/Algo/21s-pa03-peter-sun/cmake-build-debug/CMakeFiles/21s-pa03-peter-sun.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/21s-pa03-peter-sun.dir/depend

