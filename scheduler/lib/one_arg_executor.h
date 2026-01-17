#pragma once
#include "task.h"
#include "task_argument.h"

template <typename Function, typename Arg>
class OneArgExecutor : public Task {
 public:
  explicit OneArgExecutor(Function function, const Arg& argument,
                          const size_t& task_id)
      : function_(function), argument_(argument) {
    SetId(task_id);
  };

  void Calculate() override { SetValue(function_(argument_.Get())); }

 private:
  Function function_;
  TaskArgument<Arg> argument_;
};