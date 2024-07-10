# Minimum required CMake version
cmake_minimum_required(VERSION 3.6)

# CXX_version
set(CMAKE_CXX_STANDARD 20)

# Project name
project(Carinfo-Manager)

# Statically link the MSVC runtime when debugging
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND ${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

# Build standalone executable
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
set(CMAKE_EXE_LINKER_FLAGS "-static")

# third party
add_subdirectory(site-packages/cpp-httplib-0.16.0)
include_directories(site-packages/cpp-httplib-0.16.0/include)

# Add your source files here
AUX_SOURCE_DIRECTORY(src SOURCES)
list(REMOVE_ITEM SOURCES "src/server.cpp" "src/client.cpp")

# Add your header filessource directory here
include_directories(include)

# Add executable target
add_executable(Carinfo-Manager-Server src/server.cpp ${SOURCES})
target_link_libraries(Carinfo-Manager-Server libhttp)

add_executable(Carinfo-Manager-Client src/client.cpp ${SOURCES})
target_link_libraries(Carinfo-Manager-Client libhttp)

# Link Libraries
# TARGET_LINK_LIBRARIES(${PROJECT_NAME} ws2_32.lib)

# test
enable_testing()
add_subdirectory(test)

# install target
# install(TARGETS ${PROJECT_NAME} DESTINATION bin)
