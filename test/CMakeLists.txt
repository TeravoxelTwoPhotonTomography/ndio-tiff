##############################################################################
#  Testing
##############################################################################

find_package(GTEST PATHS cmake)
file(GLOB TEST_SOURCES *.cc)
if(GTEST_FOUND AND TEST_SOURCES)
  find_package(Threads)
  enable_testing()
  include_directories(${PROJECT_BINARY_DIR})
  include_directories(${GTEST_INCLUDE_DIR})
  add_executable(test-ndio-tiff ${TEST_SOURCES})
  target_link_libraries(test-ndio-tiff
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${ND_LIBRARIES}
    )
  add_dependencies(test-ndio-tiff gtest nd)
  gtest_copy_shared_libraries(test-ndio-tiff)
  install(TARGETS test-ndio-tiff EXPORT ndio-tiff-targets DESTINATION bin)
  add_test(TestNDIOTiff test-ndio-tiff)
  gtest_copy_shared_libraries(test-ndio-tiff)
endif()