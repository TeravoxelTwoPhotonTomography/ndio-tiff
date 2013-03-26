cmake_minimum_required(VERSION 2.8)
project(ndio-tiff-plugin)

macro(show v)
  message("${v} is ${${v}}")
endmacro()

################################################################################
# CONFIG
################################################################################

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
#  Documentation
###############################################################################

find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(${PROJECT_SOURCE_DIR}/doc/Doxyfile.in
                 ${PROJECT_BINARY_DIR}/doc/Doxyfile
                 @ONLY
                 )
  add_custom_target(ndio-tiff-docs
    ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/doc/Doxyfile
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    COMMENT           "Generating Docs for Tiff IO Plugin"
    VERBATIM
    )
endif()

###############################################################################
#  Install
###############################################################################
foreach(_path ${NDIO_PLUGIN_PATH})
  install(TARGETS ndio-tiff EXPORT ndio-tiff-targets LIBRARY DESTINATION ${_path})
endforeach()
install(TARGETS ndio-tiff EXPORT ndio-tiff-targets LIBRARY DESTINATION bin/plugins)
install(EXPORT ndio-tiff-targets DESTINATION cmake)
export(TARGETS ndio-tiff FILE ndio-tiff-config.cmake)
export(PACKAGE ndio-tiff)