#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "scheduler.h"

TEST(SchedulerIntTestSuite, OneTaskExecuteAllTest) {
  TTaskScheduler scheduler;
  auto id = scheduler.add([](float a, float c) { return (2 * a * c); }, 2, 13);
  scheduler.executeAll();
  ASSERT_EQ(scheduler.getResult<float>(id), 52);
}

TEST(SchedulerIntTestSuite, TwoTaskExecuteAllTest) {
  TTaskScheduler scheduler;
  float a = 23546;
  auto id1 = scheduler.add([](float a) { return (22 * a); }, 23546);
  auto id2 = scheduler.add([](float a) { return (a * a); }, 123);
  scheduler.executeAll();
  ASSERT_EQ(scheduler.getResult<float>(id1), 518012);
  ASSERT_EQ(scheduler.getResult<float>(id2), 15129);
}

TEST(SchedulerIntTestSuite, TwoDependenceExecuteAllTest) {
  TTaskScheduler scheduler;
  auto id1 = scheduler.add([](float a) { return (22 * a); }, 235);
  auto id2 = scheduler.add([](float a) { return (a * a); }, scheduler.getFutureResult<float>(id1));
  scheduler.executeAll();
  ASSERT_EQ(scheduler.getResult<float>(id2), 2.67289e+07);
}

TEST(SchedulerIntTestSuite, NoExecuteAllTaskTest) {
  TTaskScheduler scheduler;
  auto id = scheduler.add([](float a, float c) { return (a * c + 11); }, 246, 239);
  ASSERT_EQ(scheduler.getResult<float>(id), 58805);
}

int SumOfCubes(int x, int y) {
  return x*x*x + y*y*y;
}

TEST(SchedulerIntTestSuite, NotLambdaTaskTest) {
  TTaskScheduler scheduler;
  auto id = scheduler.add(SumOfCubes, 27, 59);
  ASSERT_EQ(scheduler.getResult<int>(id), 225062);
}

TEST(SchedulerIntTestSuite, VectorTaskOneArgumentTest) {
  TTaskScheduler scheduler;
  auto id = scheduler.add([](int a) { return std::vector<int>(6, a); }, 2707);
  ASSERT_EQ(scheduler.getResult<std::vector<int>>(id), std::vector<int>({2707, 2707, 2707, 2707, 2707, 2707}));
}