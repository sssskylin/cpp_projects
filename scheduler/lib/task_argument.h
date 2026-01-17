#pragma once
#include "future_result.h"
template <typename T>
class TaskArgument {
 public:
  TaskArgument() = default;
  explicit TaskArgument(const T& argument) : argument_(argument) {}
  
  T Get() {
    return argument_;
  }

 private:
  T argument_;
};

template <typename T>
class TaskArgument<FutureResult<T>> {
 public:
  TaskArgument() = default;
  explicit TaskArgument(const FutureResult<T>& argument) : argument_(argument) {}
  
  T Get() {
    return argument_.task_->template GetTaskResult<T>();
  }

 private:
  FutureResult<T> argument_;
};

