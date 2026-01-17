#include <processing.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(ReadTest, ByNewLine) {
    std::vector<std::stringstream> files(2);
    files[0] << "1\n2\n3\n4\n5";
    files[1] << "6\n7\n8\n9\n10";
    auto result = AsDataFlow(files) | Split("\n") | AsVector();
    ASSERT_THAT(result, testing::ElementsAre("1", "2", "3", "4", "5", "6", "7", "8", "9", "10"));
}

TEST(ReadTest, ByDifferentDelimiters) {
    std::vector<std::stringstream> files(3);
    files[0] << "q\nw e";
    files[1] << "r!t!y";
    files[2] << "u.i{o]p";
    auto result = AsDataFlow(files) | Split("\n.{]! ") | AsVector();
    ASSERT_THAT(result, testing::ElementsAre("q", "w", "e", "r", "t", "y", "u", "i", "o", "p"));
}

TEST(ReadTest, Empty) {
    std::vector<std::stringstream> files(1);
    files[0] << "qwe";
    auto result = AsDataFlow(files) | Split("") | AsVector();
    ASSERT_THAT(result, testing::ElementsAre("qwe"));
}

TEST(ReadTest, BySpace) {
    std::vector<std::stringstream> files(2);
    files[0] << "1 2 3 4 5";
    files[1] << "6 7 8 9 10";
    auto result = AsDataFlow(files) | Split(" ") | AsVector();
    ASSERT_THAT(result, testing::ElementsAre("1", "2", "3", "4", "5", "6", "7", "8", "9", "10"));
}
