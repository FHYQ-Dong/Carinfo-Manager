cmake_minimum_required(VERSION 3.6)

# add source files
AUX_SOURCE_DIRECTORY(../src ORIGINAL_SOURCES)
list(REMOVE_ITEM ORIGINAL_SOURCES ../src/server.cpp ../src/client.cpp)
include_directories(../include)

# Statically link the MSVC runtime when debugging
# if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND ${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
#     set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
#     set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
# endif()

# test
set(GTEST_DIR ../site-packages/googletest-1.14.0)
add_subdirectory(${GTEST_DIR} googletest-1.14.0)
include_directories(${GTEST_DIR}/googletest/include)
include_directories(${GTEST_DIR}/googlemock/include)
set(GTEST_LIB gtest gtest_main gmock gmock_main)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
include(GoogleTest)
find_package(GTest)

AUX_SOURCE_DIRECTORY(. TEST_FILES)
foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(FILE_NAME ${TEST_FILE} NAME_WE)
    add_executable(${FILE_NAME} ${TEST_FILE} ${ORIGINAL_SOURCES})
    target_link_libraries(${FILE_NAME} ${GTEST_LIB})
    target_link_libraries(${FILE_NAME} libhttp libjson)
    gtest_discover_tests(${FILE_NAME})
endforeach()
