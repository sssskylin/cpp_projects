#pragma once
#include "task.h"
#include "task_argument.h"

template <typename Function, typename Arg1, typename Arg2>
class TwoArgExecutor : public Task {
 public:
  explicit TwoArgExecutor(Function function, const Arg1& argument1,
                          const Arg2& argument2, const size_t& task_id)
      : function_(function), argument1_(argument1), argument2_(argument2) {
    SetId(task_id);
  };

  void Calculate() override {
    SetValue(function_(argument1_.Get(), argument2_.Get()));
  }

 private:
  Function function_;
  TaskArgument<Arg1> argument1_;
  TaskArgument<Arg2> argument2_;
};