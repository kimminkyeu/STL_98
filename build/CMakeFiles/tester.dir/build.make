# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Users/minkyeki/.brew/Cellar/cmake/3.24.2/bin/cmake

# The command to remove a file.
RM = /Users/minkyeki/.brew/Cellar/cmake/3.24.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /goinfre/minkyeki/mySTL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /goinfre/minkyeki/mySTL/build

# Include any dependencies generated for this target.
include CMakeFiles/tester.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/tester.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/tester.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tester.dir/flags.make

CMakeFiles/tester.dir/test/main.cpp.o: CMakeFiles/tester.dir/flags.make
CMakeFiles/tester.dir/test/main.cpp.o: /goinfre/minkyeki/mySTL/test/main.cpp
CMakeFiles/tester.dir/test/main.cpp.o: CMakeFiles/tester.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/goinfre/minkyeki/mySTL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tester.dir/test/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/tester.dir/test/main.cpp.o -MF CMakeFiles/tester.dir/test/main.cpp.o.d -o CMakeFiles/tester.dir/test/main.cpp.o -c /goinfre/minkyeki/mySTL/test/main.cpp

CMakeFiles/tester.dir/test/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tester.dir/test/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /goinfre/minkyeki/mySTL/test/main.cpp > CMakeFiles/tester.dir/test/main.cpp.i

CMakeFiles/tester.dir/test/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tester.dir/test/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /goinfre/minkyeki/mySTL/test/main.cpp -o CMakeFiles/tester.dir/test/main.cpp.s

# Object files for target tester
tester_OBJECTS = \
"CMakeFiles/tester.dir/test/main.cpp.o"

# External object files for target tester
tester_EXTERNAL_OBJECTS =

tester: CMakeFiles/tester.dir/test/main.cpp.o
tester: CMakeFiles/tester.dir/build.make
tester: CMakeFiles/tester.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/goinfre/minkyeki/mySTL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable tester"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tester.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tester.dir/build: tester
.PHONY : CMakeFiles/tester.dir/build

CMakeFiles/tester.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tester.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tester.dir/clean

CMakeFiles/tester.dir/depend:
	cd /goinfre/minkyeki/mySTL/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /goinfre/minkyeki/mySTL /goinfre/minkyeki/mySTL /goinfre/minkyeki/mySTL/build /goinfre/minkyeki/mySTL/build /goinfre/minkyeki/mySTL/build/CMakeFiles/tester.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tester.dir/depend

