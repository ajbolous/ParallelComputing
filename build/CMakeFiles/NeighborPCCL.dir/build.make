# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bolous/projects/parallel_computing

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bolous/projects/parallel_computing/build

# Include any dependencies generated for this target.
include CMakeFiles/NeighborPCCL.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/NeighborPCCL.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/NeighborPCCL.dir/flags.make

CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.o: CMakeFiles/NeighborPCCL.dir/flags.make
CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.o: ../connected_components/parallel_neighbor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bolous/projects/parallel_computing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.o -c /home/bolous/projects/parallel_computing/connected_components/parallel_neighbor.cpp

CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bolous/projects/parallel_computing/connected_components/parallel_neighbor.cpp > CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.i

CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bolous/projects/parallel_computing/connected_components/parallel_neighbor.cpp -o CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.s

CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.o.requires:

.PHONY : CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.o.requires

CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.o.provides: CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.o.requires
	$(MAKE) -f CMakeFiles/NeighborPCCL.dir/build.make CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.o.provides.build
.PHONY : CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.o.provides

CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.o.provides.build: CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.o


# Object files for target NeighborPCCL
NeighborPCCL_OBJECTS = \
"CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.o"

# External object files for target NeighborPCCL
NeighborPCCL_EXTERNAL_OBJECTS =

NeighborPCCL: CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.o
NeighborPCCL: CMakeFiles/NeighborPCCL.dir/build.make
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_stitching3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_superres3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_videostab3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_aruco3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_bgsegm3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_bioinspired3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_ccalib3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_cvv3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_dpm3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_face3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_fuzzy3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_hdf3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_img_hash3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_line_descriptor3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_optflow3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_reg3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_rgbd3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_saliency3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_stereo3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_structured_light3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_surface_matching3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_tracking3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_xfeatures2d3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_ximgproc3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_xobjdetect3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_xphoto3.so.3.3.1
NeighborPCCL: /usr/lib/x86_64-linux-gnu/libOpenCL.so
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_shape3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_photo3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_calib3d3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_viz3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_phase_unwrapping3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_video3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_datasets3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_plot3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_text3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_dnn3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_features2d3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_flann3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_highgui3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_ml3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_videoio3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_imgcodecs3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_objdetect3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_imgproc3.so.3.3.1
NeighborPCCL: /opt/ros/kinetic/lib/libopencv_core3.so.3.3.1
NeighborPCCL: CMakeFiles/NeighborPCCL.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bolous/projects/parallel_computing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable NeighborPCCL"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/NeighborPCCL.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/NeighborPCCL.dir/build: NeighborPCCL

.PHONY : CMakeFiles/NeighborPCCL.dir/build

CMakeFiles/NeighborPCCL.dir/requires: CMakeFiles/NeighborPCCL.dir/connected_components/parallel_neighbor.cpp.o.requires

.PHONY : CMakeFiles/NeighborPCCL.dir/requires

CMakeFiles/NeighborPCCL.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/NeighborPCCL.dir/cmake_clean.cmake
.PHONY : CMakeFiles/NeighborPCCL.dir/clean

CMakeFiles/NeighborPCCL.dir/depend:
	cd /home/bolous/projects/parallel_computing/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bolous/projects/parallel_computing /home/bolous/projects/parallel_computing /home/bolous/projects/parallel_computing/build /home/bolous/projects/parallel_computing/build /home/bolous/projects/parallel_computing/build/CMakeFiles/NeighborPCCL.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/NeighborPCCL.dir/depend

