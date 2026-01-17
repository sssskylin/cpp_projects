#include <unrolled_list.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>

TEST(UnrolledListTest, DefaultConstructor) {
  unrolled_list<std::string, 2> empty_list;
  EXPECT_TRUE(empty_list.empty());
  EXPECT_EQ(empty_list.size(), 0);
}

TEST(UnrolledListTest, insertChars) {
  unrolled_list<char, 2> letters;
  letters.push_back('a');
  letters.push_front('b');
  letters.push_back('c');
  letters.push_front('d');

  auto it = letters.begin();
  ++it;
  letters.insert(it, 'k');

  auto it_begin = letters.begin();
  auto it_end = letters.end();
  EXPECT_EQ(*it_begin, 'd');
  letters.pop_back();
  EXPECT_EQ(letters.back(), 'a');
  ASSERT_FALSE(letters.empty());
}

TEST(UnrolledListTest, insertStrings) {
  unrolled_list<std::string, 3> languages;
  languages.push_back("c++");
  languages.push_back("python");
  languages.push_back("rust");

  auto it = languages.begin();
  ++it;
  languages.insert(it, "kotlin");

  EXPECT_EQ(languages.size(), 4);
  EXPECT_EQ(languages.front(), "c++");
  EXPECT_EQ(languages.back(), "rust");

  auto iter = languages.begin();
  EXPECT_EQ(*iter++, "c++");
  EXPECT_EQ(*iter++, "kotlin");
  EXPECT_EQ(*iter++, "python");
  EXPECT_EQ(*iter++, "rust");
}