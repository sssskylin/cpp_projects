#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "scheduler.h"

struct Point3D {
  int x, y, z;
  bool operator==(const Point3D& other) const {
    return x == other.x && y == other.y && z == other.z;
  }
};

TEST(SchedulerDifferentTypesTestSuite, MixedReturnTypesTest) {
  TTaskScheduler scheduler;

  auto id1 = scheduler.add([]() { return 42; });
  auto id2 = scheduler.add([](double x) { return x * 1.5; }, 10.0);
  auto id3 = scheduler.add(
      [](const std::string& s) { return s + " crocodilo"; }, std::string("bombardino"));
  auto id4 =
      scheduler.add([](int a, int b) { return Point3D{a, b, a + b}; }, 3, 4);

  scheduler.executeAll();

  ASSERT_EQ(scheduler.getResult<int>(id1), 42);
  ASSERT_DOUBLE_EQ(scheduler.getResult<double>(id2), 15.0);
  ASSERT_EQ(scheduler.getResult<std::string>(id3), "bombardino crocodilo");
  ASSERT_EQ(scheduler.getResult<Point3D>(id4), (Point3D{3, 4, 7}));
}

TEST(SchedulerDifferentTypesTestSuite, TaskChainDependenciesTest) {
  TTaskScheduler scheduler;

  auto id1 = scheduler.add([](int x) { return x * 2; }, 5);
  auto id2 = scheduler.add([](int x) { return x + 10; },
                           scheduler.getFutureResult<int>(id1));
  auto id3 = scheduler.add([](int x) { return x / 2; },
                           scheduler.getFutureResult<int>(id2));
  auto id4 = scheduler.add([](int x) { return std::to_string(x); },
                           scheduler.getFutureResult<int>(id3));

  scheduler.executeAll();

  ASSERT_EQ(scheduler.getResult<int>(id1), 10);
  ASSERT_EQ(scheduler.getResult<int>(id2), 20);
  ASSERT_EQ(scheduler.getResult<int>(id3), 10);
  ASSERT_EQ(scheduler.getResult<std::string>(id4), "10");
}

TEST(SchedulerDifferentTypesTestSuite, CustomFunctionsAndObjectsTest) {
  TTaskScheduler scheduler;

  auto to_upper = [](const std::string& s) {
    std::string result = s;
    for (auto& c : result) c = toupper(c);
    return result;
  };

  auto concat_points = [](const Point3D& p1, const Point3D& p2) {
    return Point3D{p1.x + p2.x, p1.y + p2.y, p1.z + p2.z};
  };

  auto id1 = scheduler.add(to_upper, std::string("loser=lover"));
  auto id2 = scheduler.add(concat_points, Point3D{1, 2, 3}, Point3D{4, 5, 6});

  scheduler.executeAll();

  ASSERT_EQ(scheduler.getResult<std::string>(id1), "LOSER=LOVER");
  ASSERT_EQ(scheduler.getResult<Point3D>(id2), (Point3D{5, 7, 9}));
}

TEST(SchedulerDifferentTypesTestSuite, ExceptionHandlingTest) {
  TTaskScheduler scheduler;

  auto throwing_task = []() {
    throw std::runtime_error("Test error");
    return 42;
  };

  auto id = scheduler.add(throwing_task);

  EXPECT_THROW(
      {
        scheduler.executeAll();
        scheduler.getResult<int>(id);
      },
      std::runtime_error);
}

TEST(SchedulerDifferentTypesTestSuite, MixedTaskTypesTest) {
  TTaskScheduler scheduler;

  auto id1 = scheduler.add([]() { return 3.14159; });
  auto id2 = scheduler.add([](int x) { return x * x; }, 5);
  auto id3 = scheduler.add([](double a, double b) { return a + b; }, 2.5, 3.5);
  auto id4 = scheduler.add([](double x, double y) { return x * y; },
                           scheduler.getFutureResult<double>(id1),
                           scheduler.getFutureResult<double>(id3));

  scheduler.executeAll();

  ASSERT_DOUBLE_EQ(scheduler.getResult<double>(id1), 3.14159);
  ASSERT_EQ(scheduler.getResult<int>(id2), 25);
  ASSERT_DOUBLE_EQ(scheduler.getResult<double>(id3), 6.0);
  ASSERT_DOUBLE_EQ(scheduler.getResult<double>(id4), 3.14159 * 6.0);
}
