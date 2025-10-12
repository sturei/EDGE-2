#
# Project properties
#
cmake_minimum_required(VERSION 4.1) # earliest version tested
project(EDGE-2 VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


##################
# Dependencies
##################

#
# GoogleTest. See http://google.github.io/googletest/quickstart-cmake.html
#
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

##################
# Sources
##################

set(SOURCES
    src/utils/hello.test.cpp
    src/utils/graph.test.cpp
    src/utils/vec3d.test.cpp
    src/utils/geom3d.test.cpp
    src/brep/body.test.cpp
    src/document/model.test.cpp
    src/document/store.test.cpp
    src/document/document.test.cpp
)


##################
# Targets
##################

add_executable(e2_test
  ${SOURCES}
)
target_include_directories(e2_test
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)

################
# Testing
################

enable_testing()

target_link_libraries(e2_test
  GTest::gtest_main
  GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(e2_test)