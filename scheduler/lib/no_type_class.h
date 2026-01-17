#pragma once
class NoTypeClass {
 public:
  virtual NoTypeClass* copy() = 0;
  virtual ~NoTypeClass() = default;
};
