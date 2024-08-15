# Minimum required CMake version
cmake_minimum_required(VERSION 3.6)

# CXX_version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project name
project(Carinfo-Manager LANGUAGES CXX)

# Qt configuration
set(CMAKE_PREFIX_PATH "c:/Program Files/Qt/6.7.1/msvc2019_64") # Qt Kit Dir. Change it with your own Qt Kit Dir
set(CMAKE_AUTOMOC ON)
find_package(Qt6 COMPONENTS Widgets REQUIRED) # Qt COMPONENTS

# Specify MSVC UTF-8 encoding   
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# Static linking the libraries
if (WIN32 AND MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
elseif (WIN32 AND MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++ -mwindows")
elseif (LINUX)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

# third party
add_subdirectory(site-packages/cpp-httplib-0.16.0)
include_directories(site-packages/cpp-httplib-0.16.0/include)
add_subdirectory(site-packages/nlohmann-json-3.11.3)
include_directories(site-packages/nlohmann-json-3.11.3/include)
include_directories(site-packages/spdlog-1.14.1/include)

# Add your source files here
aux_source_directory(src SOURCES)
file(GLOB QT_INCLUDES "qt-include/carinfo-manager/*.hpp")
aux_source_directory(qt-src QT_SOURCES)
list(REMOVE_ITEM SOURCES "src/server-main.cpp" "src/client-main.cpp")

# Add executable target
add_executable(Carinfo-Manager-Server src/server-main.cpp ${SOURCES})
target_include_directories(Carinfo-Manager-Server PUBLIC include)
target_link_libraries(Carinfo-Manager-Server http json)

add_executable(Carinfo-Manager-Client WIN32 src/client-main.cpp ${QT_INCLUDES} ${SOURCES} ${QT_SOURCES})
target_include_directories(Carinfo-Manager-Client PUBLIC include qt-include)
target_link_libraries(
    Carinfo-Manager-Client
    http
    json
    Qt6::Widgets
)
