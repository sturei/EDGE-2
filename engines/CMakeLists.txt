#
# Project properties
#
cmake_minimum_required(VERSION 4.1) # earliest version tested
project(EDGE-2 VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

##################
# Dependencies
# This works for cmake. If VSCode (with CMake Tools) extension can't find the new include paths, try closing and reopening the workspace (close the window, new window, open recent)
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

#
# nlohmann/json
# See https://json.nlohmann.me/integration/cmake
#
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)


##################
# Sources
##################

set(MODELLING_SERVICE_SOURCES
    src/brep/body.cpp
    src/brep/brep.fixtures.cpp
    src/brep/bnearpoint.cpp
    src/brep/bnormal.cpp
    src/brep/brepModel.cpp
    src/brep/containment.cpp
    src/brep/segment.cpp
    src/brep/navigate.cpp
    src/brep/bnearpoint.cpp
    src/brep/tessellate.cpp
    src/document/document.cpp
    src/document/document.actions.cpp
    src/document/documentService.cpp
    src/document/model.cpp
    src/document/store.cpp
    src/frep/frep.fixtures.cpp
    src/frep/fobject.cpp
    src/frep/fevaluate.cpp
    src/frep/fnavigate.cpp
    src/frep/frepModel.cpp
    src/frep/fsdf.cpp
    src/scene/sceneActions.cpp
    src/shape/shapeModel.cpp
    src/shape/shape.actions.cpp
    src/utils/vec3d.cpp
    src/utils/evaluate.cpp
    src/utils/nearpoint.cpp   
    src/utils/parameterize.cpp
    src/utils/polygonize.cpp
    src/utils/solve.cpp
)

set(TEST_SOURCES
    src/utils/hello.test.cpp
    src/utils/graph.test.cpp
    src/utils/vec3d.test.cpp
    src/utils/geom3d.test.cpp
    src/utils/ray3d.test.cpp
    src/utils/pla3d.test.cpp
    src/utils/sph3d.test.cpp
    src/utils/cir3d.test.cpp
    src/utils/evaluate.test.cpp
    src/utils/parameterize.test.cpp
    src/utils/polygonize.test.cpp
    src/brep/body.test.cpp
    src/brep/brepModel.test.cpp
    src/brep/containment.test.cpp
    src/brep/navigate.test.cpp
    src/brep/tessellate.test.cpp
    src/frep/fobject.test.cpp
    src/frep/fnavigate.test.cpp
    src/frep/fevaluate.test.cpp
    src/frep/fsdf.test.cpp
    src/document/store.test.cpp
    src/document/document.test.cpp
    src/document/document.actions.test.cpp
    src/document/documentService.test.cpp
)

##################
# Targets
##################

#
# testing
#
add_executable(e2_test
  ${TEST_SOURCES}
  ${MODELLING_SERVICE_SOURCES}
)
target_include_directories(e2_test
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(e2_test
  PRIVATE
    GTest::gtest_main
    GTest::gmock_main
    nlohmann_json::nlohmann_json
)

#
# modelling service
#
add_executable(e2_modellingService
    src/main/modellingService_main.cpp
    ${MODELLING_SERVICE_SOURCES}
)
target_include_directories(e2_modellingService
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(e2_modellingService
  PRIVATE
    nlohmann_json::nlohmann_json
)

################
# Testing
################

#
# Enable testing with GoogleTest and CTest
#
enable_testing()
include(GoogleTest)
gtest_discover_tests(e2_test)