#pragma once
#include "task.h"

template <typename Function>
class ZeroArgExecutor : public Task {
 public:
  explicit ZeroArgExecutor(Function function, const size_t& task_id)
      : function_(function) {
    SetId(task_id);
  };

  void Calculate() override { SetValue(function_()); }

 private:
  Function function_;
};