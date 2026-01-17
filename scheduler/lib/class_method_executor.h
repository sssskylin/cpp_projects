#pragma once
#include "task.h"
#include "task_argument.h"

template <typename Function, typename Object, typename T, typename Arg>
class ClassMethodExecutor : public Task {
 public:
  using ClassMethod = Function (Object::*)(T) const;
  explicit ClassMethodExecutor(ClassMethod method, const Object& object,
                               const Arg& argument, const size_t& task_id)
      : method_(method), object_(object), argument_(argument) {
    SetId(task_id);
  };

  void Calculate() override {
    SetValue((object_.*method_)(argument_.Get()));
  }

 private:
  ClassMethod method_;
  Object object_;
  TaskArgument<Arg> argument_;
};