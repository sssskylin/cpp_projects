#pragma once
#include <optional>
#include <typeinfo>

template <typename Container>
class DataFlow {
 public:
  using iterator = Container::iterator;
  using const_iterator = Container::const_iterator;
  using value_type = typename std::iterator_traits<iterator>::value_type;

  DataFlow(Container& it) : it_(it.begin()), end_(it.end()) {}

  const_iterator begin() const { return it_; }
  const_iterator end() const { return end_; }

 private:
 const_iterator it_;
 const_iterator end_;
};