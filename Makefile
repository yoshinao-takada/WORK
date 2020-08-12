# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/yoshinao/REPOS/WORK

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yoshinao/REPOS/WORK

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/yoshinao/REPOS/WORK/CMakeFiles /home/yoshinao/REPOS/WORK//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/yoshinao/REPOS/WORK/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named base_l

# Build rule for target.
base_l: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 base_l
.PHONY : base_l

# fast build rule for target.
base_l/fast:
	$(MAKE) $(MAKESILENT) -f base_l/CMakeFiles/base_l.dir/build.make base_l/CMakeFiles/base_l.dir/build
.PHONY : base_l/fast

#=============================================================================
# Target rules for targets named base_g

# Build rule for target.
base_g: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 base_g
.PHONY : base_g

# fast build rule for target.
base_g/fast:
	$(MAKE) $(MAKESILENT) -f base_g/CMakeFiles/base_g.dir/build.make base_g/CMakeFiles/base_g.dir/build
.PHONY : base_g/fast

#=============================================================================
# Target rules for targets named BL_matc128CM.o

# Build rule for target.
BL_matc128CM.o: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 BL_matc128CM.o
.PHONY : BL_matc128CM.o

# fast build rule for target.
BL_matc128CM.o/fast:
	$(MAKE) $(MAKESILENT) -f base_g/CMakeFiles/BL_matc128CM.o.dir/build.make base_g/CMakeFiles/BL_matc128CM.o.dir/build
.PHONY : BL_matc128CM.o/fast

#=============================================================================
# Target rules for targets named BL_matc64CM.o

# Build rule for target.
BL_matc64CM.o: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 BL_matc64CM.o
.PHONY : BL_matc64CM.o

# fast build rule for target.
BL_matc64CM.o/fast:
	$(MAKE) $(MAKESILENT) -f base_g/CMakeFiles/BL_matc64CM.o.dir/build.make base_g/CMakeFiles/BL_matc64CM.o.dir/build
.PHONY : BL_matc64CM.o/fast

#=============================================================================
# Target rules for targets named BL_matr64CM.o

# Build rule for target.
BL_matr64CM.o: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 BL_matr64CM.o
.PHONY : BL_matr64CM.o

# fast build rule for target.
BL_matr64CM.o/fast:
	$(MAKE) $(MAKESILENT) -f base_g/CMakeFiles/BL_matr64CM.o.dir/build.make base_g/CMakeFiles/BL_matr64CM.o.dir/build
.PHONY : BL_matr64CM.o/fast

#=============================================================================
# Target rules for targets named BL_matc128RM.o

# Build rule for target.
BL_matc128RM.o: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 BL_matc128RM.o
.PHONY : BL_matc128RM.o

# fast build rule for target.
BL_matc128RM.o/fast:
	$(MAKE) $(MAKESILENT) -f base_g/CMakeFiles/BL_matc128RM.o.dir/build.make base_g/CMakeFiles/BL_matc128RM.o.dir/build
.PHONY : BL_matc128RM.o/fast

#=============================================================================
# Target rules for targets named BL_matc64RM.o

# Build rule for target.
BL_matc64RM.o: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 BL_matc64RM.o
.PHONY : BL_matc64RM.o

# fast build rule for target.
BL_matc64RM.o/fast:
	$(MAKE) $(MAKESILENT) -f base_g/CMakeFiles/BL_matc64RM.o.dir/build.make base_g/CMakeFiles/BL_matc64RM.o.dir/build
.PHONY : BL_matc64RM.o/fast

#=============================================================================
# Target rules for targets named BL_matr32CM.o

# Build rule for target.
BL_matr32CM.o: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 BL_matr32CM.o
.PHONY : BL_matr32CM.o

# fast build rule for target.
BL_matr32CM.o/fast:
	$(MAKE) $(MAKESILENT) -f base_g/CMakeFiles/BL_matr32CM.o.dir/build.make base_g/CMakeFiles/BL_matr32CM.o.dir/build
.PHONY : BL_matr32CM.o/fast

#=============================================================================
# Target rules for targets named BL_matr64RM.o

# Build rule for target.
BL_matr64RM.o: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 BL_matr64RM.o
.PHONY : BL_matr64RM.o

# fast build rule for target.
BL_matr64RM.o/fast:
	$(MAKE) $(MAKESILENT) -f base_g/CMakeFiles/BL_matr64RM.o.dir/build.make base_g/CMakeFiles/BL_matr64RM.o.dir/build
.PHONY : BL_matr64RM.o/fast

#=============================================================================
# Target rules for targets named BL_matr32RM.o

# Build rule for target.
BL_matr32RM.o: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 BL_matr32RM.o
.PHONY : BL_matr32RM.o

# fast build rule for target.
BL_matr32RM.o/fast:
	$(MAKE) $(MAKESILENT) -f base_g/CMakeFiles/BL_matr32RM.o.dir/build.make base_g/CMakeFiles/BL_matr32RM.o.dir/build
.PHONY : BL_matr32RM.o/fast

#=============================================================================
# Target rules for targets named base_o

# Build rule for target.
base_o: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 base_o
.PHONY : base_o

# fast build rule for target.
base_o/fast:
	$(MAKE) $(MAKESILENT) -f base_o/CMakeFiles/base_o.dir/build.make base_o/CMakeFiles/base_o.dir/build
.PHONY : base_o/fast

#=============================================================================
# Target rules for targets named UTbase_l

# Build rule for target.
UTbase_l: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 UTbase_l
.PHONY : UTbase_l

# fast build rule for target.
UTbase_l/fast:
	$(MAKE) $(MAKESILENT) -f UTbase_l/CMakeFiles/UTbase_l.dir/build.make UTbase_l/CMakeFiles/UTbase_l.dir/build
.PHONY : UTbase_l/fast

#=============================================================================
# Target rules for targets named UTbase_g

# Build rule for target.
UTbase_g: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 UTbase_g
.PHONY : UTbase_g

# fast build rule for target.
UTbase_g/fast:
	$(MAKE) $(MAKESILENT) -f UTbase_g/CMakeFiles/UTbase_g.dir/build.make UTbase_g/CMakeFiles/UTbase_g.dir/build
.PHONY : UTbase_g/fast

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... BL_matc128CM.o"
	@echo "... BL_matc128RM.o"
	@echo "... BL_matc64CM.o"
	@echo "... BL_matc64RM.o"
	@echo "... BL_matr32CM.o"
	@echo "... BL_matr32RM.o"
	@echo "... BL_matr64CM.o"
	@echo "... BL_matr64RM.o"
	@echo "... UTbase_g"
	@echo "... UTbase_l"
	@echo "... base_g"
	@echo "... base_l"
	@echo "... base_o"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

