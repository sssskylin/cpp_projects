#pragma once
#include "any.h"

class Task {
 public:
  Task() = default;
  virtual ~Task() = default;
  
  virtual void Calculate() = 0;

  template<typename T>
  T GetTaskResult() {
    if (res_value_.HasValue()) {
      return res_value_.Get<T>();
    }
    Calculate();
    return res_value_.Get<T>();
  } 

  template<typename T>
  void SetValue(T&& new_value) {
    res_value_ = new_value;
  }

  void SetId(const size_t& id) {
    id_ = id;
  }
 private:
  Any res_value_;
  size_t id_;
};