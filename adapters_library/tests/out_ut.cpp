#include <processing.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(OutTest, Out) {
    std::vector<int> input = {1, 2, 3, 4, 5};
    std::stringstream file_emulator;
    auto result = AsDataFlow(input) | Out(file_emulator);
    ASSERT_EQ(file_emulator.str(), "12345");
}