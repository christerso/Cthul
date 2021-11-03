# CMakeList.txt : CMake project for Chtuhl, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project(kings)
set(CMAKE_ARGS "-Wno-dev")
set(CMAKE_CXX_STANDARD 17)
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0A00 -D_WINVER=0x0A00 -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DPOCO_NO_AUTOMATIC_LIBS=1 -D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING=1")

#-DGLOG_NO_ABBREVIATED_SEVERITIES=1
add_custom_target(copy-resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/resources" "${CMAKE_BINARY_DIR}/resources")

if (MSVC)
    set(VCPKG_INSTALLS ${Z_VCPKG_ROOT_DIR}/installed/x64-windows)
    set (GLOG_ENTRY "glog::glog")
else()
    find_package(Threads REQUIRED)
    set (GLOG_ENTRY "glog")
endif()

make_directory (${CMAKE_BINARY_DIR}/logging)

add_executable (kings "kings.cpp" "kings.h" "resourcemanager.h" "resourcemanager.cpp" "astar.cpp" "astar.h" "player.cpp" "player.h" "common.h" "kingdom.h" "kingdom.cpp"  "movable.h" "army.h" "army.cpp" "character.h" "character.cpp" "castle.h" "castle.cpp" "position.h" "threadmanager.h" "threadmanager.cpp")
add_custom_target(copy-runtime-files ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources)

set(CMAKE_MAKE_PROGRAM ninja)

set(Boost_NO_WARN_NEW_VERSIONS ON)
set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_MULTITHREADED  ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread filesystem)
find_package(glog CONFIG REQUIRED)
find_package(libpng CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Flatbuffers CONFIG REQUIRED)
find_package(Poco CONFIG REQUIRED COMPONENTS Zip Util)
find_package(GTest REQUIRED)
find_package(ZLIB REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-gfx CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)
find_package(sdl2-net CONFIG REQUIRED)
find_path(BSHOSHANY_THREAD_POOL_INCLUDE_DIRS "thread_pool.hpp")
find_path(RAPIDCSV_INCLUDE_DIRS "rapidcsv.h")
include_directories(${GTEST_INCLUDE_DIR})
enable_testing()

add_subdirectory(tests)

include_directories(
  ${GLOG_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIR}
  ${LIBNOISE_INCLUDE_DIR}
  ${GTEST_INCLUDE_DIRS})

target_include_directories(kings PRIVATE ${BSHOSHANY_THREAD_POOL_INCLUDE_DIRS} ${RAPIDCSV_INCLUDE_DIRS})
target_link_libraries(kings PRIVATE
                      SDL2::SDL2_image
                      SDL2::SDL2_net
                      SDL2::SDL2_ttf
                      SDL2::SDL2_gfx
                      SDL2::SDL2 SDL2::SDL2main
                      nlohmann_json nlohmann_json::nlohmann_json
                      png
                      Poco::Zip
                      ${LIBNOISE_LIBRARIES}
                      ${Boost_LIBRARIES}
                      ${GLOG_ENTRY}
                      ZLIB::ZLIB
                      bcrypt
                      flatbuffers::flatbuffers
                      fmt::fmt
                      fmt::fmt-header-only
                      Boost::boost
                      glog::glog
                      )