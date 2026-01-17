#pragma once
#include <optional>
#include <stdexcept>
#include <typeinfo>

template <typename Function, typename FlowType>
class FilterAdapter {
 public:
  using value_type = typename std::decay_t<FlowType>::value_type;

  explicit FilterAdapter(const Function& func, const FlowType& flow)
      : func_(func), flow_(flow) {}

  class FilterIterator {
   public:
    using iterator_category = std::input_iterator_tag;
    using value_type = value_type;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using FlowIterator = typename FlowType::const_iterator;

    FilterIterator(FlowIterator it, FlowIterator end, const Function* func)
        : it_(it), end_(end), func_(func) {
      Filter();
    }

    FilterIterator& operator++() {
      ++it_;
      Filter();
      return *this;
    }

    reference operator*() const { return *it_; }

    bool operator!=(const FilterIterator& other) const {
      return it_ != other.it_;
    }
    bool operator==(const FilterIterator& other) const {
      return it_ == other.it_;
    }

   private:
    void Filter() {
      while (it_ != end_ && !((*func_)(*it_))) {
        ++it_;
      }
    }
    FlowIterator it_;
    FlowIterator end_;
    const Function* func_;
  };

  using const_iterator = FilterIterator;

  FilterIterator begin() const {
    return FilterIterator(flow_.begin(), flow_.end(), &func_);
  }
  FilterIterator end() const {
    return FilterIterator(flow_.end(), flow_.end(), nullptr);
  }

 private:
  Function func_;
  const FlowType& flow_;
};