# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.18.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.18.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/smarsu/tencent/cc/smcc_201229

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/smarsu/tencent/cc/smcc_201229/build

# Include any dependencies generated for this target.
include core/CMakeFiles/smcc_core.dir/depend.make

# Include the progress variables for this target.
include core/CMakeFiles/smcc_core.dir/progress.make

# Include the compile flags for this target's objects.
include core/CMakeFiles/smcc_core.dir/flags.make

core/CMakeFiles/smcc_core.dir/reader.cc.o: core/CMakeFiles/smcc_core.dir/flags.make
core/CMakeFiles/smcc_core.dir/reader.cc.o: ../core/reader.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/smarsu/tencent/cc/smcc_201229/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object core/CMakeFiles/smcc_core.dir/reader.cc.o"
	cd /Users/smarsu/tencent/cc/smcc_201229/build/core && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/smcc_core.dir/reader.cc.o -c /Users/smarsu/tencent/cc/smcc_201229/core/reader.cc

core/CMakeFiles/smcc_core.dir/reader.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/smcc_core.dir/reader.cc.i"
	cd /Users/smarsu/tencent/cc/smcc_201229/build/core && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/smarsu/tencent/cc/smcc_201229/core/reader.cc > CMakeFiles/smcc_core.dir/reader.cc.i

core/CMakeFiles/smcc_core.dir/reader.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/smcc_core.dir/reader.cc.s"
	cd /Users/smarsu/tencent/cc/smcc_201229/build/core && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/smarsu/tencent/cc/smcc_201229/core/reader.cc -o CMakeFiles/smcc_core.dir/reader.cc.s

core/CMakeFiles/smcc_core.dir/expr.cc.o: core/CMakeFiles/smcc_core.dir/flags.make
core/CMakeFiles/smcc_core.dir/expr.cc.o: ../core/expr.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/smarsu/tencent/cc/smcc_201229/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object core/CMakeFiles/smcc_core.dir/expr.cc.o"
	cd /Users/smarsu/tencent/cc/smcc_201229/build/core && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/smcc_core.dir/expr.cc.o -c /Users/smarsu/tencent/cc/smcc_201229/core/expr.cc

core/CMakeFiles/smcc_core.dir/expr.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/smcc_core.dir/expr.cc.i"
	cd /Users/smarsu/tencent/cc/smcc_201229/build/core && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/smarsu/tencent/cc/smcc_201229/core/expr.cc > CMakeFiles/smcc_core.dir/expr.cc.i

core/CMakeFiles/smcc_core.dir/expr.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/smcc_core.dir/expr.cc.s"
	cd /Users/smarsu/tencent/cc/smcc_201229/build/core && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/smarsu/tencent/cc/smcc_201229/core/expr.cc -o CMakeFiles/smcc_core.dir/expr.cc.s

core/CMakeFiles/smcc_core.dir/ast.cc.o: core/CMakeFiles/smcc_core.dir/flags.make
core/CMakeFiles/smcc_core.dir/ast.cc.o: ../core/ast.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/smarsu/tencent/cc/smcc_201229/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object core/CMakeFiles/smcc_core.dir/ast.cc.o"
	cd /Users/smarsu/tencent/cc/smcc_201229/build/core && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/smcc_core.dir/ast.cc.o -c /Users/smarsu/tencent/cc/smcc_201229/core/ast.cc

core/CMakeFiles/smcc_core.dir/ast.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/smcc_core.dir/ast.cc.i"
	cd /Users/smarsu/tencent/cc/smcc_201229/build/core && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/smarsu/tencent/cc/smcc_201229/core/ast.cc > CMakeFiles/smcc_core.dir/ast.cc.i

core/CMakeFiles/smcc_core.dir/ast.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/smcc_core.dir/ast.cc.s"
	cd /Users/smarsu/tencent/cc/smcc_201229/build/core && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/smarsu/tencent/cc/smcc_201229/core/ast.cc -o CMakeFiles/smcc_core.dir/ast.cc.s

core/CMakeFiles/smcc_core.dir/codegen.cc.o: core/CMakeFiles/smcc_core.dir/flags.make
core/CMakeFiles/smcc_core.dir/codegen.cc.o: ../core/codegen.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/smarsu/tencent/cc/smcc_201229/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object core/CMakeFiles/smcc_core.dir/codegen.cc.o"
	cd /Users/smarsu/tencent/cc/smcc_201229/build/core && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/smcc_core.dir/codegen.cc.o -c /Users/smarsu/tencent/cc/smcc_201229/core/codegen.cc

core/CMakeFiles/smcc_core.dir/codegen.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/smcc_core.dir/codegen.cc.i"
	cd /Users/smarsu/tencent/cc/smcc_201229/build/core && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/smarsu/tencent/cc/smcc_201229/core/codegen.cc > CMakeFiles/smcc_core.dir/codegen.cc.i

core/CMakeFiles/smcc_core.dir/codegen.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/smcc_core.dir/codegen.cc.s"
	cd /Users/smarsu/tencent/cc/smcc_201229/build/core && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/smarsu/tencent/cc/smcc_201229/core/codegen.cc -o CMakeFiles/smcc_core.dir/codegen.cc.s

# Object files for target smcc_core
smcc_core_OBJECTS = \
"CMakeFiles/smcc_core.dir/reader.cc.o" \
"CMakeFiles/smcc_core.dir/expr.cc.o" \
"CMakeFiles/smcc_core.dir/ast.cc.o" \
"CMakeFiles/smcc_core.dir/codegen.cc.o"

# External object files for target smcc_core
smcc_core_EXTERNAL_OBJECTS =

core/libsmcc_core.a: core/CMakeFiles/smcc_core.dir/reader.cc.o
core/libsmcc_core.a: core/CMakeFiles/smcc_core.dir/expr.cc.o
core/libsmcc_core.a: core/CMakeFiles/smcc_core.dir/ast.cc.o
core/libsmcc_core.a: core/CMakeFiles/smcc_core.dir/codegen.cc.o
core/libsmcc_core.a: core/CMakeFiles/smcc_core.dir/build.make
core/libsmcc_core.a: core/CMakeFiles/smcc_core.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/smarsu/tencent/cc/smcc_201229/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX static library libsmcc_core.a"
	cd /Users/smarsu/tencent/cc/smcc_201229/build/core && $(CMAKE_COMMAND) -P CMakeFiles/smcc_core.dir/cmake_clean_target.cmake
	cd /Users/smarsu/tencent/cc/smcc_201229/build/core && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/smcc_core.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
core/CMakeFiles/smcc_core.dir/build: core/libsmcc_core.a

.PHONY : core/CMakeFiles/smcc_core.dir/build

core/CMakeFiles/smcc_core.dir/clean:
	cd /Users/smarsu/tencent/cc/smcc_201229/build/core && $(CMAKE_COMMAND) -P CMakeFiles/smcc_core.dir/cmake_clean.cmake
.PHONY : core/CMakeFiles/smcc_core.dir/clean

core/CMakeFiles/smcc_core.dir/depend:
	cd /Users/smarsu/tencent/cc/smcc_201229/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/smarsu/tencent/cc/smcc_201229 /Users/smarsu/tencent/cc/smcc_201229/core /Users/smarsu/tencent/cc/smcc_201229/build /Users/smarsu/tencent/cc/smcc_201229/build/core /Users/smarsu/tencent/cc/smcc_201229/build/core/CMakeFiles/smcc_core.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : core/CMakeFiles/smcc_core.dir/depend

