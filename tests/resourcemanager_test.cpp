#include "../ResourceManager.h"
#include "../ResourceManager.cpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <SDL2/SDL.h>
#include <string>


class ResourceManagerMock : public RM::ResourceManager
{
public:

    ResourceManagerMock(): ResourceManager::ResourceManager() {}
    MOCK_METHOD2(load_image, SDL_Texture*(std::string name, std::string filename));
};

TEST(ResourceManagerMockTest, load)
{
   ResourceManagerMock mock;
   //EXPECT_FALSE(resource_mock.load("test", "anything"));
   EXPECT_TRUE(mock.load_image("Cthuhlu", "resources/images/cthulhu.png") != nullptr);
}

TEST(ResourceManagerMocktest, load_image)
{
    ResourceManagerMock resource_mock;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}