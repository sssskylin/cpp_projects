#pragma once
#include "task.h"

template <typename T>
struct FutureResult {
  Task *task_;
  explicit FutureResult(Task *task) : task_(task) {}
};