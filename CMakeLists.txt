cmake_minimum_required(VERSION 3.22)

if (DEFINED ENV{VCPKG_ROOT})
    message("Using global vcpkg")
    string(REPLACE "\\" "/" VCPKG_ROOT_FORMATED $ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT_FORMATED}/scripts/buildsystems/vcpkg.cmake")
else ()
    message("Using vcpkg as project's submodule")
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/lib/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif ()
include(${CMAKE_TOOLCHAIN_FILE})

project(zeromq)

set(CMAKE_CXX_STANDARD 11)

#  Boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost 1.77.0 REQUIRED COMPONENTS filesystem)

# OpenCV
find_package(OpenCV REQUIRED)

# ZeroMQ
find_package(cppzmq REQUIRED)

add_executable(server server.cpp)

find_package(nlohmann_json CONFIG REQUIRED)

target_include_directories(server PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        )

target_link_libraries(server PRIVATE
        ${Boost_LIBRARIES}
        ${OpenCV_LIBS}
        cppzmq
        nlohmann_json::nlohmann_json
        )

add_executable(client client.cpp)

target_include_directories(client PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        )

target_link_libraries(client PRIVATE
        ${Boost_LIBRARIES}
        ${OpenCV_LIBS}
        cppzmq
        nlohmann_json::nlohmann_json
        )
