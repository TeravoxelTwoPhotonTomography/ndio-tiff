cmake_minimum_required(VERSION 2.8)
project(ndio-tiff-plugin)

macro(show v)
  message("${v} is ${${v}}")
endmacro()

################################################################################
# CONFIG
################################################################################
set(NDIO_TIFF_TEST_DATA_PATH ${PROJECT_SOURCE_DIR}/test/data)
configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/config.h)

find_package(ND    CONFIG PATHS cmake)
find_package(MYLIB CONFIG PATHS cmake)

include_directories(${ND_INCLUDE_DIRS})
include_directories(${MYLIB_INCLUDE_DIR})

get_directory_property(ND_SOURCE_DIR PARENT_DIRECTORY)

################################################################################
# SOURCE
################################################################################

file(GLOB SRCS src/*.c)
file(GLOB HDRS src/*.h)

################################################################################
# TARGETS
################################################################################

add_library(ndio-tiff MODULE ${SRCS} ${HDRS})
target_link_libraries(ndio-tiff ${MYLIB_LIBRARIES} ${ND_LIBRARIES})
add_dependencies(ndio-tiff nd libmylib)

###############################################################################
#  Tests
###############################################################################
add_subdirectory(test)

###############################################################################
#  Documentation
###############################################################################
add_subdirectory(doc)

###############################################################################
#  Install
###############################################################################
foreach(_path ${NDIO_PLUGIN_PATH})
  install(TARGETS ndio-tiff DESTINATION ${_path})
endforeach()
install(TARGETS ndio-tiff EXPORT ndio-tiff-targets DESTINATION bin/plugins)
install(EXPORT ndio-tiff-targets DESTINATION cmake)
export(TARGETS ndio-tiff FILE ndio-tiff-config.cmake)
export(PACKAGE ndio-tiff)