# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/xuan/Documents/Networked and Distributed Systems/Simple-HTTP-Server/echo"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/xuan/Documents/Networked and Distributed Systems/Simple-HTTP-Server/echo/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/echo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/echo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/echo.dir/flags.make

CMakeFiles/echo.dir/main.c.o: CMakeFiles/echo.dir/flags.make
CMakeFiles/echo.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/xuan/Documents/Networked and Distributed Systems/Simple-HTTP-Server/echo/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/echo.dir/main.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/echo.dir/main.c.o   -c "/Users/xuan/Documents/Networked and Distributed Systems/Simple-HTTP-Server/echo/main.c"

CMakeFiles/echo.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/echo.dir/main.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/xuan/Documents/Networked and Distributed Systems/Simple-HTTP-Server/echo/main.c" > CMakeFiles/echo.dir/main.c.i

CMakeFiles/echo.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/echo.dir/main.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/xuan/Documents/Networked and Distributed Systems/Simple-HTTP-Server/echo/main.c" -o CMakeFiles/echo.dir/main.c.s

CMakeFiles/echo.dir/main.c.o.requires:

.PHONY : CMakeFiles/echo.dir/main.c.o.requires

CMakeFiles/echo.dir/main.c.o.provides: CMakeFiles/echo.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/echo.dir/build.make CMakeFiles/echo.dir/main.c.o.provides.build
.PHONY : CMakeFiles/echo.dir/main.c.o.provides

CMakeFiles/echo.dir/main.c.o.provides.build: CMakeFiles/echo.dir/main.c.o


CMakeFiles/echo.dir/echo_client.c.o: CMakeFiles/echo.dir/flags.make
CMakeFiles/echo.dir/echo_client.c.o: ../echo_client.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/xuan/Documents/Networked and Distributed Systems/Simple-HTTP-Server/echo/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/echo.dir/echo_client.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/echo.dir/echo_client.c.o   -c "/Users/xuan/Documents/Networked and Distributed Systems/Simple-HTTP-Server/echo/echo_client.c"

CMakeFiles/echo.dir/echo_client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/echo.dir/echo_client.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/xuan/Documents/Networked and Distributed Systems/Simple-HTTP-Server/echo/echo_client.c" > CMakeFiles/echo.dir/echo_client.c.i

CMakeFiles/echo.dir/echo_client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/echo.dir/echo_client.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/xuan/Documents/Networked and Distributed Systems/Simple-HTTP-Server/echo/echo_client.c" -o CMakeFiles/echo.dir/echo_client.c.s

CMakeFiles/echo.dir/echo_client.c.o.requires:

.PHONY : CMakeFiles/echo.dir/echo_client.c.o.requires

CMakeFiles/echo.dir/echo_client.c.o.provides: CMakeFiles/echo.dir/echo_client.c.o.requires
	$(MAKE) -f CMakeFiles/echo.dir/build.make CMakeFiles/echo.dir/echo_client.c.o.provides.build
.PHONY : CMakeFiles/echo.dir/echo_client.c.o.provides

CMakeFiles/echo.dir/echo_client.c.o.provides.build: CMakeFiles/echo.dir/echo_client.c.o


# Object files for target echo
echo_OBJECTS = \
"CMakeFiles/echo.dir/main.c.o" \
"CMakeFiles/echo.dir/echo_client.c.o"

# External object files for target echo
echo_EXTERNAL_OBJECTS =

echo: CMakeFiles/echo.dir/main.c.o
echo: CMakeFiles/echo.dir/echo_client.c.o
echo: CMakeFiles/echo.dir/build.make
echo: CMakeFiles/echo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/xuan/Documents/Networked and Distributed Systems/Simple-HTTP-Server/echo/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable echo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/echo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/echo.dir/build: echo

.PHONY : CMakeFiles/echo.dir/build

CMakeFiles/echo.dir/requires: CMakeFiles/echo.dir/main.c.o.requires
CMakeFiles/echo.dir/requires: CMakeFiles/echo.dir/echo_client.c.o.requires

.PHONY : CMakeFiles/echo.dir/requires

CMakeFiles/echo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/echo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/echo.dir/clean

CMakeFiles/echo.dir/depend:
	cd "/Users/xuan/Documents/Networked and Distributed Systems/Simple-HTTP-Server/echo/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/xuan/Documents/Networked and Distributed Systems/Simple-HTTP-Server/echo" "/Users/xuan/Documents/Networked and Distributed Systems/Simple-HTTP-Server/echo" "/Users/xuan/Documents/Networked and Distributed Systems/Simple-HTTP-Server/echo/cmake-build-debug" "/Users/xuan/Documents/Networked and Distributed Systems/Simple-HTTP-Server/echo/cmake-build-debug" "/Users/xuan/Documents/Networked and Distributed Systems/Simple-HTTP-Server/echo/cmake-build-debug/CMakeFiles/echo.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/echo.dir/depend

