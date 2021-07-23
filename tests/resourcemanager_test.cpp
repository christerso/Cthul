#include "../ResourceManager.h"
#include "../ResourceManager.cpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <SDL2/SDL_surface.h>
#include <string>


class ResourceManagerMock : public RM::ResourceManager
{
public:
    MOCK_METHOD2(load, SDL_Surface*(std::string name, std::string filename));
    MOCK_METHOD2(load_image, SDL_Surface*(std::string name, std::string filename));
    MOCK_METHOD2(load_level, SDL_Surface*(std::string name, std::string filename));
};

TEST(ResourceManagerMockTest, load)
{
   ResourceManagerMock resource_mock;
   //EXPECT_FALSE(resource_mock.load("test", "anything"));
   EXPECT_TRUE(resource_mock.load("Globe", "globe.png"));
}

TEST(ResourceManagerMocktest, load_image)
{
    ResourceManagerMock resource_mock;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}