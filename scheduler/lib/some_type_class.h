#pragma once
#include "no_type_class.h"

template <typename Type>
class SomeTypeClass : public NoTypeClass {
 public:
  SomeTypeClass(const Type &object) : object(object) {}

  NoTypeClass* copy() override { return new SomeTypeClass<Type>(object); }

  Type object;
};