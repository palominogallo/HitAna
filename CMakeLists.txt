cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(CMAKE_BUILD_TYPE Release)

# Set the project name and version
project(hitana VERSION 1.0.0)

find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
include_directories(${ROOT_INCLUDE_DIRS})

#EDEP-SIM
LINK_DIRECTORIES($ENV{EDEP_ROOT}/$ENV{EDEP_TARGET}/lib)
include_directories($ENV{EDEP_ROOT}/$ENV{EDEP_TARGET}/include)

add_subdirectory(hitanalib)
add_subdirectory(make_histos)

install(FILES setup.sh DESTINATION .)
