#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "scheduler.h"

TEST(SchedulerStringTestSuite, StringTaskOneArgumentTest) {
  TTaskScheduler scheduler;
  auto id = scheduler.add([](std::string a) { return a.size(); },
                          std::string("tralalero tralala"));
  ASSERT_EQ(scheduler.getResult<size_t>(id), 17);
}

TEST(SchedulerStringTestSuite, StringTaskTwoArgumentTest) {
  TTaskScheduler scheduler;
  auto id = scheduler.add([](std::string a, std::string b) { return a + b; },
                          std::string("ballerina"), std::string(" cappuccina"));
  ASSERT_EQ(scheduler.getResult<std::string>(id), "ballerina cappuccina");
}

TEST(SchedulerStringTestSuite, VectorTaskOneArgumentTest) {
  TTaskScheduler scheduler;
  auto to_lower = [](const std::string& s) {
    std::string result = s;
    for (auto& c : result) c = tolower(c);
    return result;
  };
  auto id = scheduler.add(to_lower, std::string("MIMIMIMI"));
  ASSERT_EQ(scheduler.getResult<std::string>(id), "mimimimi");
}

TEST(SchedulerStringTestSuite, OnlyFunctionTest) {
  TTaskScheduler scheduler;
  auto id = scheduler.add([]() { return "only function test"; });
  ASSERT_EQ(scheduler.getResult<const char*>(id), "only function test");
}

TEST(SchedulerStringTestSuite, DependenceStringTest) {
  TTaskScheduler scheduler;
  auto id1 = scheduler.add([]() { return "a"; });
  auto id2 = scheduler.add([](std::string a, std::string b) { return a + b; },
                           scheduler.getFutureResult<const char*>(id1), std::string("mo"));
  auto id3 = scheduler.add([](std::string a, std::string b) { return a + b; },
                           scheduler.getFutureResult<std::string>(id2), std::string("gus"));
  ASSERT_EQ(scheduler.getResult<std::string>(id3), "amogus");
}

TEST(SchedulerStringTestSuite, DiamondDependenceStringTest) {
  TTaskScheduler scheduler;
  auto id1 = scheduler.add([]() { return "VE"; });
  auto id2 = scheduler.add([](std::string a, std::string b) { return a + b; },
                           std::string("LO"), scheduler.getFutureResult<const char*>(id1));
  auto id3 = scheduler.add([](std::string a, std::string b) { return a + b; },
                           std::string(" DI"), scheduler.getFutureResult<const char*>(id1));
  auto id4 = scheduler.add([](std::string a, std::string b) { return a + b; },
                           scheduler.getFutureResult<std::string>(id2),
                           scheduler.getFutureResult<std::string>(id3));
  ASSERT_EQ(scheduler.getResult<std::string>(id4), "LOVE DIVE");
}