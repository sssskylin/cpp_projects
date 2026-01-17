#pragma once
#include <iostream>
#include <memory>
#include <utility>
#include "no_type_class.h"
#include "some_type_class.h"

class Any {
 public:
  Any() = default;

  template <typename Type>
  explicit Any(const Type &other) : ptr(new SomeTypeClass<Type>(other)) {}
  
  Any(const Any &other) : ptr(other.ptr->copy()) {}
  ~Any() { delete ptr; };

  template <typename Type>
  Any &operator=(const Type &other) {
    delete ptr;
    ptr = new SomeTypeClass<Type>(other);
    return *this;
  }

  template <typename Type>
  Type Get() {
    return static_cast<SomeTypeClass<Type> *>(ptr)->object;
  }

  bool HasValue() { return ptr != nullptr; }

 private:
  NoTypeClass* ptr = nullptr;
};