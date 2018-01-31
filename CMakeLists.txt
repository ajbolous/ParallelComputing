cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)
project(ParallelComputing)


find_package(OpenCV REQUIRED)
find_package(OpenCL REQUIRED)


add_executable(ParallelReduce parallel_reduce/parallel_reduce.cpp)
target_link_libraries(ParallelReduce ${OpenCL_LIBRARY})
configure_file(parallel_reduce/psr.cl psr.cl COPYONLY)

add_executable(TwoStageSerial connected_components/serial_twostage.cpp)
target_link_libraries(TwoStageSerial ${OpenCV_LIBS})

add_executable(NeighborPCCL connected_components/parallel_neighbor.cpp)
target_link_libraries(NeighborPCCL ${OpenCV_LIBS})
target_link_libraries(NeighborPCCL ${OpenCL_LIBRARY})

add_executable(CameraPCLL connected_components/camera_pccl.cpp)
target_link_libraries(CameraPCLL ${OpenCV_LIBS})
target_link_libraries(CameraPCLL ${OpenCL_LIBRARY})

add_executable(CameraTwoStage connected_components/camera_serial.cpp)
target_link_libraries(CameraTwoStage ${OpenCV_LIBS})
target_link_libraries(CameraTwoStage ${OpenCL_LIBRARY})



configure_file(connected_components/pccl.cl pccl.cl COPYONLY)
configure_file(connected_components/test.jpg test.jpg COPYONLY)
configure_file(connected_components/test2.jpg test2.jpg COPYONLY)
configure_file(connected_components/test3.jpg test3.jpg COPYONLY)
configure_file(connected_components/test4.jpg test4.jpg COPYONLY)