#pragma once
#include <algorithm>
#include <expected>
#include <utility>
#include <vector>

template <typename FlowType>
class SplitExpectedAdapter {
 public:
  using FlowIterator = typename FlowType::const_iterator;
  using ExpectedType = typename std::iterator_traits<FlowIterator>::value_type;
  using GoodType = typename ExpectedType::value_type;
  using BadType = typename ExpectedType::error_type;

  SplitExpectedAdapter(const FlowType& flow) : flow_(flow) {
    auto it = flow_.begin();
    while (it != flow_.end()) {
      if ((*it).has_value()) {
        good_.push_back((*it).value());
      } else {
        bad_.push_back((*it).error());
        std::cout << bad_[bad_.size() - 1] << std::endl;
      }
      ++it;
    }
  }

  auto GetPair() {
    return std::make_pair(bad_, good_);
  }

 private:
  const FlowType& flow_;
  mutable std::vector<GoodType> good_;
  mutable std::vector<BadType> bad_;
};
