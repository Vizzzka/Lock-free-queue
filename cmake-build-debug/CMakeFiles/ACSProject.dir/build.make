# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.14

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2019.2.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2019.2.2\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\usach\CLionProjects\ACSProject

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\usach\CLionProjects\ACSProject\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ACSProject.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ACSProject.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ACSProject.dir/flags.make

CMakeFiles/ACSProject.dir/main.cpp.obj: CMakeFiles/ACSProject.dir/flags.make
CMakeFiles/ACSProject.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\usach\CLionProjects\ACSProject\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ACSProject.dir/main.cpp.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\ACSProject.dir\main.cpp.obj -c C:\Users\usach\CLionProjects\ACSProject\main.cpp

CMakeFiles/ACSProject.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ACSProject.dir/main.cpp.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\usach\CLionProjects\ACSProject\main.cpp > CMakeFiles\ACSProject.dir\main.cpp.i

CMakeFiles/ACSProject.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ACSProject.dir/main.cpp.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\usach\CLionProjects\ACSProject\main.cpp -o CMakeFiles\ACSProject.dir\main.cpp.s

CMakeFiles/ACSProject.dir/tests.cpp.obj: CMakeFiles/ACSProject.dir/flags.make
CMakeFiles/ACSProject.dir/tests.cpp.obj: ../tests.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\usach\CLionProjects\ACSProject\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ACSProject.dir/tests.cpp.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\ACSProject.dir\tests.cpp.obj -c C:\Users\usach\CLionProjects\ACSProject\tests.cpp

CMakeFiles/ACSProject.dir/tests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ACSProject.dir/tests.cpp.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\usach\CLionProjects\ACSProject\tests.cpp > CMakeFiles\ACSProject.dir\tests.cpp.i

CMakeFiles/ACSProject.dir/tests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ACSProject.dir/tests.cpp.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\usach\CLionProjects\ACSProject\tests.cpp -o CMakeFiles\ACSProject.dir\tests.cpp.s

# Object files for target ACSProject
ACSProject_OBJECTS = \
"CMakeFiles/ACSProject.dir/main.cpp.obj" \
"CMakeFiles/ACSProject.dir/tests.cpp.obj"

# External object files for target ACSProject
ACSProject_EXTERNAL_OBJECTS =

ACSProject.exe: CMakeFiles/ACSProject.dir/main.cpp.obj
ACSProject.exe: CMakeFiles/ACSProject.dir/tests.cpp.obj
ACSProject.exe: CMakeFiles/ACSProject.dir/build.make
ACSProject.exe: CMakeFiles/ACSProject.dir/linklibs.rsp
ACSProject.exe: CMakeFiles/ACSProject.dir/objects1.rsp
ACSProject.exe: CMakeFiles/ACSProject.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\usach\CLionProjects\ACSProject\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ACSProject.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\ACSProject.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ACSProject.dir/build: ACSProject.exe

.PHONY : CMakeFiles/ACSProject.dir/build

CMakeFiles/ACSProject.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\ACSProject.dir\cmake_clean.cmake
.PHONY : CMakeFiles/ACSProject.dir/clean

CMakeFiles/ACSProject.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\usach\CLionProjects\ACSProject C:\Users\usach\CLionProjects\ACSProject C:\Users\usach\CLionProjects\ACSProject\cmake-build-debug C:\Users\usach\CLionProjects\ACSProject\cmake-build-debug C:\Users\usach\CLionProjects\ACSProject\cmake-build-debug\CMakeFiles\ACSProject.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ACSProject.dir/depend

