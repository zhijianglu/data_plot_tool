cmake_minimum_required(VERSION 3.13)
project(data_plot)

set(CMAKE_CXX_STANDARD 11)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

set(${PYTHONHOME} ${PYTHON_INCLUDE_DIRS})
include_directories(${PYTHONHOME}/include)
include_directories(${PYTHONHOME}/Lib/site-packages/numpy/core/include)
link_directories(${PYTHONHOME}/libs)

add_definitions(-DMATPLOTLIBCPP_PYTHON_HEADER=Python.h)

#add_executable(plot_mode0 plot_mode0.cpp)
#target_link_libraries(plot_mode0 ${PYTHON_LIBRARIES})




link_directories(${PYTHONHOME}/libs)
add_definitions(-DMATPLOTLIBCPP_PYTHON_HEADER=Python.h)
add_executable(data_plot plot_mode1.cpp)
target_link_libraries(data_plot ${PYTHON_LIBRARIES})





add_executable(euroc2tum euroc2tum.cpp)
target_link_libraries(euroc2tum ${PYTHON_LIBRARIES})