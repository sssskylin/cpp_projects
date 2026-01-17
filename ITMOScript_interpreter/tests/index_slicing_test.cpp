#include <gtest/gtest.h>
#include <lib/interpreter.h>

TEST(IndexingSlicingTest, IndexingWithAssignment) {
  std::string code = R"(


        list = [1, 2, 3]


        push(list, 4)
        print(list)
        print("|")

        pop(list)
        print(list)
        print("|")

        insert(list, 1, 99)
        print(list)
        print("|")

        remove(list, 1)
        print(list)
    )";

  std::string expected = "[1, 2, 3]|[1, 2, 3]|[1, 2, 3]|[1, 2, 3]";

  std::istringstream input(code);
  std::ostringstream output;

  ASSERT_TRUE(interpret(input, output));
  ASSERT_EQ(output.str(), expected);
}


TEST(InterpreterTest, IF_ONE) {
    std::istringstream in(R"(
        if 1 == 1 then
           println("yes")
        else
           println("no")
        end if
    )");
    std::ostringstream out;
    EXPECT_TRUE(interpret(in, out));
    EXPECT_EQ(out.str(), "yes\n");
}

TEST(InterpreterTest, IF_TWO) {
    std::istringstream in(R"(
        if 1 == 0 then
           println("yes")
        else
           println("no")
        end if
    )");
    std::ostringstream out;
    EXPECT_TRUE(interpret(in, out));
    EXPECT_EQ(out.str(), "no\n");
}


TEST(InterpreterTest, INDEX) {
    std::istringstream in("a = \"ABC\" \n d = a[1] \n print(d)");
    std::ostringstream out;
    bool ok = interpret(in, out);
    EXPECT_TRUE(ok);
    EXPECT_EQ(out.str(), "B");
}

TEST(InterpreterTest, SLICE) {
    std::istringstream in("a = \"ABCDEFG\" \n d = a[1 : 4] \n print(d)");
    std::ostringstream out;
    bool ok = interpret(in, out);
    EXPECT_TRUE(ok);
    EXPECT_EQ(out.str(), "BCDE");
}

TEST(InterpreterTest, SLICE_WITHOUT_START) {
    std::istringstream in("a = \"ABCDEFG\" \n d = a[: 4] \n print(d)");
    std::ostringstream out;
    bool ok = interpret(in, out);
    EXPECT_TRUE(ok);
    EXPECT_EQ(out.str(), "ABCDE");
}

TEST(InterpreterTest, SLICE_WITHOUT_END) {
    std::istringstream in("a = \"ABCDEFG\" \n d = a[2 :] \n print(d)");
    std::ostringstream out;
    bool ok = interpret(in, out);
    EXPECT_TRUE(ok);
    EXPECT_EQ(out.str(), "CDEFG");
}

TEST(InterpreterTest, SLICE_WITHOUT_STERT_END) {
    std::istringstream in("a = \"ABCDEFG\" \n d = a[:] \n print(d)");
    std::ostringstream out;
    bool ok = interpret(in, out);
    EXPECT_TRUE(ok);
    EXPECT_EQ(out.str(), "ABCDEFG");
}

TEST(InterpreterTest, UNARY_OPERATION) {
    std::istringstream in("a = 5 \n d = -a \n print(d)");
    std::ostringstream out;
    bool ok = interpret(in, out);
    EXPECT_TRUE(ok);
    EXPECT_EQ(out.str(), "-5");
}