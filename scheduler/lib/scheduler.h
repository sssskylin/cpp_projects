#pragma once
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "any.h"
#include "class_method_executor.h"
#include "one_arg_executor.h"
#include "two_arg_executor.h"
#include "zero_arg_executor.h"

class TTaskScheduler {
 public:
  TTaskScheduler() = default;
  ~TTaskScheduler() = default;

  template <typename Function>
  size_t add(Function function) {
    size_t new_task_id = tasks_.size();
    std::shared_ptr<Task> new_task =
        std::make_shared<ZeroArgExecutor<Function>>(function, new_task_id);
    tasks_.emplace_back(new_task);
    return new_task_id;
  }

  template <typename Function, typename Arg>
  size_t add(Function function, const Arg& argument) {
    size_t new_task_id = tasks_.size();
    std::shared_ptr<Task> new_task =
        std::make_shared<OneArgExecutor<Function, Arg>>(
            function, argument, new_task_id);
    tasks_.emplace_back(new_task);
    return new_task_id;
  }

  template <typename Function, typename Arg1, typename Arg2>
  size_t add(Function function, const Arg1& argument1, const Arg2& argument2) {
    size_t new_task_id = tasks_.size();
    std::shared_ptr<Task> new_task =
        std::make_shared<TwoArgExecutor<Function, Arg1, Arg2>>(
            function, argument1, argument2, new_task_id);
    tasks_.emplace_back(new_task);
    return new_task_id;
  }

  template <typename Function, typename Object, typename T, typename Arg>
  size_t add(Function (Object::*method)(T) const, const Object& object, const Arg& argument) {
    size_t new_task_id = tasks_.size();
    std::shared_ptr<Task> new_task =
        std::make_shared<ClassMethodExecutor<Function, Object, T, Arg>>(
            method, object, argument, new_task_id);
    tasks_.emplace_back(new_task);
    return new_task_id;
  }

  template <typename T>
  FutureResult<T> getFutureResult(const size_t& task_id) {
    if (task_id >= tasks_.size()) {
      throw std::out_of_range("Task ID is out of bounds");
    }

    if (!tasks_[task_id]) {
      throw std::runtime_error("Task pointer is null");
    }

    return FutureResult<T>(tasks_[task_id].get());
  }

  template <typename T>
  T getResult(const size_t& task_id) {
    if (task_id >= tasks_.size()) {
      throw std::out_of_range("Task ID is out of bounds");
    }

    if (!tasks_[task_id]) {
      throw std::runtime_error("Task pointer is null");
    }
    return tasks_[task_id]->template GetTaskResult<T>();
  }

  void executeAll() {
    for (auto& task : tasks_) {
      task->Calculate();
    }
  }

 private:
  std::vector<std::shared_ptr<Task>> tasks_;
};