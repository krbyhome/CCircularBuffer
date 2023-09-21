cmake_minimum_required(VERSION 3.0.0)
project(lab8 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include_directories ("${PROJECT_SOURCE_DIR}/lib")
add_subdirectory(bin)

enable_testing()
add_subdirectory(tests)