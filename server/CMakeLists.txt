# Minimum required CMake version
cmake_minimum_required(VERSION 3.15)

# CXX_version
set(CMAKE_CXX_STANDARD 20)

# Project name
project(Carinfo-Manager-Server)

# Build standalone executable
# set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
# set(CMAKE_EXE_LINKER_FLAGS "-static")

# Add your source files here
AUX_SOURCE_DIRECTORY(src SOURCES)
list(REMOVE_ITEM SOURCES "src/server.cpp")

# Add your header filessource directory here
include_directories(include)

# Add executable target
add_executable(${PROJECT_NAME} src/server.cpp ${SOURCES})

# Link Libraries
# TARGET_LINK_LIBRARIES(${PROJECT_NAME} ws2_32.lib)

# test
enable_testing()
add_subdirectory(test)

# install target
# install(TARGETS ${PROJECT_NAME} DESTINATION bin)
