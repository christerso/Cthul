cmake_minimum_required (VERSION 3.8)

set(BINARY Cthultest)

add_executable(${BINARY} "../tests/ResourceManagerTest.h" "../tests/ResourceManagerTest.cpp")
add_test(NAME ${BINARY} COMMAND ${BINARY})
target_include_directories(${BINARY} PRIVATE ${RAPIDXML_INCLUDE_DIRS} rapidjson)
target_link_libraries(${BINARY} PUBLIC ${GTEST_LIBRARIES}
                      freeimage::FreeImage
                      freeimage::FreeImagePlus
                      SDL2::SDL2 SDL2::SDL2main
                      Poco::Zip
                      ${LIBNOISE_LIBRARIES}
                      ${Boost_LIBRARIES}
                      ${GLOG_ENTRY}
                      bcrypt
                      flatbuffers::flatbuffers
                      fmt::fmt
                      fmt::fmt-header-only
                      Boost::boost
                      glog::glog
                      GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main
                      )