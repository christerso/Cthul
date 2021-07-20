# CMakeList.txt : CMake project for Chtuhl, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project(Cthul)

set(CMAKE_CXX_STANDARD 17)
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
message ("CMAKE Module path: ${CMAKE_MODULE_PATH}")
message ("Current source directory is: ${Chtuhl_SOURCE_DIR}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0A00 -D_WINVER=0x0A00 -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DPOCO_NO_AUTOMATIC_LIBS=1 -D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING=1")

#-DGLOG_NO_ABBREVIATED_SEVERITIES=1
add_custom_target(copy-resources-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/Debug)

if (WIN32)
    set(VCPKG_INSTALLS ${Z_VCPKG_ROOT_DIR}/installed/x64-windows)
    set (GLOG_ENTRY "glog::glog")
else()
    find_package(Threads REQUIRED)
    set (GLOG_ENTRY "glog")
endif()

make_directory (${CMAKE_BINARY_DIR}/logging)

add_executable (Ctuhl "Cthul.cpp" "Cthul.h" "SDL2Wrapper.cpp" "SDL2Wrapper.h" "ResourceManager.h" "ResourceManager.cpp")

set(CMAKE_MAKE_PROGRAM ninja)

set(Boost_NO_WARN_NEW_VERSIONS ON)
set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_MULTITHREADED  ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost REQUIRED COMPONENTS system thread filesystem)
find_package(glog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Flatbuffers CONFIG REQUIRED)
find_package(Poco CONFIG REQUIRED COMPONENTS Zip Util)
find_package(RapidJSON CONFIG REQUIRED)
find_package(GTest REQUIRED)
find_package(ZLIB REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-gfx CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)
find_package(sdl2-net CONFIG REQUIRED)
find_package(freeimage CONFIG REQUIRED)

find_path(RAPIDXML_NS_INCLUDE_DIRS "rapidxml-ns/rapidxml_ns.hpp")
include_directories(${GTEST_INCLUDE_DIR})
find_path(RAPIDXML_NS_INCLUDE_DIRS "rapidxml-ns/rapidxml_ns.hpp")
enable_testing()

add_subdirectory(tests)

target_include_directories(Ctuhl PRIVATE ${RAPIDXML_INCLUDE_DIRS} rapidjson)
include_directories(
  ${GLOG_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIR}
  ${RAPIDJSON_INCLUDE_DIRS}
  ${LIBNOISE_INCLUDE_DIR}
  ${GTEST_INCLUDE_DIRS}
  ${RAPIDXML_NS_INCLUDE_DIRS})


target_link_libraries(Ctuhl PRIVATE
                      freeimage::FreeImage
                      freeimage::FreeImagePlus
                      SDL2::SDL2_net
                      SDL2::SDL2_ttf
                      SDL2::SDL2_gfx
                      SDL2::SDL2 SDL2::SDL2main
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