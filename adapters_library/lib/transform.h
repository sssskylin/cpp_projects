#pragma once
#include <iterator>
#include <optional>
#include <type_traits>

template <typename Function, typename FlowType>
class TransformAdapter {
 public:
  using flow_value_type = typename std::decay_t<FlowType>::value_type;
  using value_type =
      decltype(std::declval<Function>()(std::declval<flow_value_type>()));

  TransformAdapter(const Function& func, const FlowType& flow)
      : func_(func), flow_(flow) {}

  class TransformIterator {
   public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using FlowIterator = typename FlowType::const_iterator;
    using value_type = decltype(std::declval<Function>()(*std::declval<FlowIterator>()));

    TransformIterator(FlowIterator it, FlowIterator end, const Function& func)
        : it_(it), end_(end), func_(func) {
      if (it_ != end_) {
        current_ = (func_)(*it_);
      }
    }

    TransformIterator& operator++() {
      ++it_;
      if (it_ != end_) {
        current_ = (func_)(*it_);
      } else {
        current_.reset();
      }
      return *this;
    }
    
    reference operator*() const { return *current_; }

    bool operator!=(const TransformIterator& other) const {
      return it_ != other.it_;
    }
    bool operator==(const TransformIterator& other) const {
      return it_ == other.it_;
    }

   private:
    FlowIterator it_;
    FlowIterator end_;
    Function func_;
    std::optional<value_type> current_;
  };

  using const_iterator = TransformIterator;

  TransformIterator begin() const {
    return TransformIterator(flow_.begin(), flow_.end(), func_);
  }
  TransformIterator end() const {
    return TransformIterator(flow_.end(), flow_.end(), func_);
  }

 private:
  Function func_;
  const FlowType& flow_;
};