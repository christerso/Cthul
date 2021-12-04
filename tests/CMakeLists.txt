cmake_minimum_required (VERSION 3.8)

set(BINARY kingstest)

set(CMAKE_MAKE_PROGRAM ninja)
set(CMAKE_CXX_STANDARD 20)
set(Boost_NO_WARN_NEW_VERSIONS ON)
set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_MULTITHREADED  ON)
set(Boost_USE_STATIC_RUNTIME ON)

if(MSVC)
    set(VCPKG_INSTALLS ${Z_VCPKG_ROOT_DIR}/installed/x64-windows)
    set (GLOG_ENTRY "glog::glog")
    set (BCRYPT_LIBRARY "bcrypt")
    set (PNG_LIBRARY "png")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBoost_NO_WARN_NEW_VERSIONS=1 -D_WIN32_WINNT=0x0A00 -D_WINVER=0x0A00 -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DPOCO_NO_AUTOMATIC_LIBS=1 -D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING=1")
else(MSVC)
    set (PNG_LIBRARY "png_static")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBoost_NO_WARN_NEW_VERSIONS=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DPOCO_NO_AUTOMATIC_LIBS=1 -D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING=1")
    find_package(Threads REQUIRED)
    set (GLOG_ENTRY "glog")
    set (BCRYPT_LIBRARY "")
endif(MSVC)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread filesystem)
find_package(glog CONFIG REQUIRED)
find_package(libpng CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Flatbuffers CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-gfx CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)
find_package(sdl2-net CONFIG REQUIRED)
find_path(RAPIDCSV_INCLUDE_DIRS "rapidcsv.h")
add_executable(${BINARY} "resourcemanager_test.h" "resourcemanager_test.cpp")

include_directories(
  ${GLOG_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIR}
  ${GTEST_INCLUDE_DIRS})

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PUBLIC
                      SDL2::SDL2 SDL2::SDL2main
                      ${Boost_LIBRARIES}
                      ${GLOG_ENTRY}
                      SDL2::SDL2_image
                      SDL2::SDL2_net
                      SDL2::SDL2_ttf
                      SDL2::SDL2_gfx
                      SDL2::SDL2 SDL2::SDL2main
                      flatbuffers::flatbuffers
                      fmt::fmt
                      ${BCRYPT_LIBRARY}
                      fmt::fmt-header-only
                      glog::glog
                      ${PNG_LIBRARY} PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main
                      )