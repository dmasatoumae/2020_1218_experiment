cmake_minimum_required(VERSION 2.8)
project(main)
set(CMAKE_CXX_STANDARD 14)
find_package(Eigen3 REQUIRED)

include_directories(EIGEN3_INCLUDE_DIR)
add_executable(main main3.cpp)
target_link_libraries(main Eigen3::Eigen)
