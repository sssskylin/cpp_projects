#pragma once
#include <unordered_map>
#include <utility>
#include <vector>
template <typename Container, typename KeyFunc, typename Aggregator,
          typename Accumulator>
class AggregateByKeyAdapter {
 public:
  using InputType = typename Container::value_type;
  using KeyType = std::invoke_result_t<KeyFunc, InputType>;
  using ResultType = std::pair<KeyType, Accumulator>;
  using value_type = ResultType;

  AggregateByKeyAdapter(const Container& container, Accumulator init_value,
                        Aggregator aggregator, KeyFunc key_func_)
      : init_value_(init_value), aggregator_(aggregator), key_func_(key_func_) {
    std::unordered_map<KeyType, Accumulator> accumulators;
    std::vector<KeyType> order;

    for (const InputType& item : container) {
      KeyType key = key_func_(item);
      if (accumulators.find(key) == accumulators.end()) {
        accumulators[key] = init_value_;
        order.push_back(key);
      }
      aggregator_(item, accumulators[key]);
    }

    for (const KeyType& key : order) {
      result_.push_back(std::make_pair(key, accumulators[key]));
    }
  }

  class AggregateByKeyIterator {
   public:
    using iterator_category = std::input_iterator_tag;
    using value_type = ResultType;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using FlowIterator = std::vector<ResultType>::const_iterator;

    AggregateByKeyIterator(FlowIterator it)
        : it_(it) {}

    reference operator*() const { return *it_; }

    AggregateByKeyIterator& operator++() {
      ++it_;
      return *this;
    }

    bool operator==(const AggregateByKeyIterator& other) const {
      return it_ == other.it_;
    }
    bool operator!=(const AggregateByKeyIterator& other) const {
      return it_ != other.it_;
    }

   private:
    FlowIterator it_;
  };

  using const_iterator = AggregateByKeyIterator;

  AggregateByKeyIterator begin() const {
    return AggregateByKeyIterator(result_.begin());
  }
  AggregateByKeyIterator end() const {
    return AggregateByKeyIterator(result_.end());
  }

 private:
  Accumulator init_value_;
  Aggregator aggregator_;
  KeyFunc key_func_;
  std::vector<ResultType> result_;
};