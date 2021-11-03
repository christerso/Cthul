cmake_minimum_required (VERSION 3.8)

set(BINARY kingstest)

add_executable(${BINARY} "resourcemanager_test.h" "resourcemanager_test.cpp")

include_directories(
  ${GLOG_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIR}
  ${RAPIDJSON_INCLUDE_DIRS}
  ${LIBNOISE_INCLUDE_DIR}
  ${GTEST_INCLUDE_DIRS})

add_test(NAME ${BINARY} COMMAND ${BINARY})
target_link_libraries(${BINARY} PUBLIC ${GTEST_LIBRARIES}
                      SDL2::SDL2 SDL2::SDL2main
                      Poco::Zip
                      ${LIBNOISE_LIBRARIES}
                      ${Boost_LIBRARIES}
                      ${GLOG_ENTRY}
                      SDL2::SDL2_image
                      SDL2::SDL2_net
                      SDL2::SDL2_ttf
                      SDL2::SDL2_gfx
                      SDL2::SDL2 SDL2::SDL2main
                      bcrypt
                      flatbuffers::flatbuffers
                      fmt::fmt
                      fmt::fmt-header-only
                      Boost::boost
                      glog::glog
                      GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main
                      )