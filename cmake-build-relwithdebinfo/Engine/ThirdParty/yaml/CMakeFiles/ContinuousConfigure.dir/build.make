# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /opt/clion/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/i-use-gentoo-btw/Documents/UntitledVulkanGameEngine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/i-use-gentoo-btw/Documents/UntitledVulkanGameEngine/cmake-build-relwithdebinfo

# Utility rule file for ContinuousConfigure.

# Include any custom commands dependencies for this target.
include Engine/ThirdParty/yaml/CMakeFiles/ContinuousConfigure.dir/compiler_depend.make

# Include the progress variables for this target.
include Engine/ThirdParty/yaml/CMakeFiles/ContinuousConfigure.dir/progress.make

Engine/ThirdParty/yaml/CMakeFiles/ContinuousConfigure:
	cd /home/i-use-gentoo-btw/Documents/UntitledVulkanGameEngine/cmake-build-relwithdebinfo/Engine/ThirdParty/yaml && /opt/clion/bin/cmake/linux/bin/ctest -D ContinuousConfigure

ContinuousConfigure: Engine/ThirdParty/yaml/CMakeFiles/ContinuousConfigure
ContinuousConfigure: Engine/ThirdParty/yaml/CMakeFiles/ContinuousConfigure.dir/build.make
.PHONY : ContinuousConfigure

# Rule to build all files generated by this target.
Engine/ThirdParty/yaml/CMakeFiles/ContinuousConfigure.dir/build: ContinuousConfigure
.PHONY : Engine/ThirdParty/yaml/CMakeFiles/ContinuousConfigure.dir/build

Engine/ThirdParty/yaml/CMakeFiles/ContinuousConfigure.dir/clean:
	cd /home/i-use-gentoo-btw/Documents/UntitledVulkanGameEngine/cmake-build-relwithdebinfo/Engine/ThirdParty/yaml && $(CMAKE_COMMAND) -P CMakeFiles/ContinuousConfigure.dir/cmake_clean.cmake
.PHONY : Engine/ThirdParty/yaml/CMakeFiles/ContinuousConfigure.dir/clean

Engine/ThirdParty/yaml/CMakeFiles/ContinuousConfigure.dir/depend:
	cd /home/i-use-gentoo-btw/Documents/UntitledVulkanGameEngine/cmake-build-relwithdebinfo && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/i-use-gentoo-btw/Documents/UntitledVulkanGameEngine /home/i-use-gentoo-btw/Documents/UntitledVulkanGameEngine/Engine/ThirdParty/yaml /home/i-use-gentoo-btw/Documents/UntitledVulkanGameEngine/cmake-build-relwithdebinfo /home/i-use-gentoo-btw/Documents/UntitledVulkanGameEngine/cmake-build-relwithdebinfo/Engine/ThirdParty/yaml /home/i-use-gentoo-btw/Documents/UntitledVulkanGameEngine/cmake-build-relwithdebinfo/Engine/ThirdParty/yaml/CMakeFiles/ContinuousConfigure.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Engine/ThirdParty/yaml/CMakeFiles/ContinuousConfigure.dir/depend

