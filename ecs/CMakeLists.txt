# Notice name prefix of this variable, set by CMake according
# to value given with "project()" in the root CMakeLists.txt.

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${EcsLibrary_SOURCE_DIR}/ecs)
add_library(ecs Entity.cpp Manager.cpp System.cpp)