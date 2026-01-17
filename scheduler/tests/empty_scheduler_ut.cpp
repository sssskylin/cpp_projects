#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "scheduler.h"

TEST(EmptySchedulerTestSuite, ExceptionTest) {
  TTaskScheduler scheduler;

  ASSERT_THROW(scheduler.getFutureResult<float>(1), std::out_of_range);
}
