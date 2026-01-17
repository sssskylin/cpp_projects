#pragma once
#include <algorithm>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <utility>

template <typename Key, typename Value>
struct KV {
  Key key;
  Value value;

  bool operator==(const KV& other) const = default;
};

template <typename Base, typename Joined>
struct JoinResult {
  Base base;
  std::optional<Joined> joined;

  bool operator==(const JoinResult& other) const = default;
};

template <typename LeftFlow, typename RightFlow, typename LeftKeyExtractor,
          typename RightKeyExtractor, typename LeftValueExtractor,
          typename RightValueExtractor>
class JoinAdapter {
 public:
  using LeftFlowType = typename std::decay_t<LeftFlow>;
  using RightFlowType = typename std::decay_t<RightFlow>;

  using LeftKeyType = decltype(std::declval<LeftKeyExtractor>()(
      std::declval<typename LeftFlowType::value_type>()));
  using RightKeyType = decltype(std::declval<RightKeyExtractor>()(
      std::declval<typename RightFlowType::value_type>()));
  using BaseType = decltype(std::declval<LeftValueExtractor>()(
      std::declval<typename LeftFlowType::value_type>()));
  using JoinedType = decltype(std::declval<RightValueExtractor>()(
      std::declval<typename RightFlowType::value_type>()));

  using ResultType = JoinResult<BaseType, JoinedType>;

  JoinAdapter(LeftFlow left, RightFlow right, LeftKeyExtractor left_key_ex,
              RightKeyExtractor right_key_ex, LeftValueExtractor left_value_ex,
              RightValueExtractor right_value_ex)
      : left_flow_(left),
        right_(right),
        left_key_ex_(left_key_ex),
        right_key_ex_(right_key_ex),
        left_value_ex_(left_value_ex),
        right_value_ex_(right_value_ex) {
    BuildRightMap();
  }

  class JoinIterator {
   public:
    using iterator_category = std::input_iterator_tag;
    using value_type = ResultType;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    // JoinIterator(){};
    JoinIterator(typename LeftFlowType::const_iterator left_flow_it,
                 typename LeftFlowType::const_iterator left_flow_end,
                 const std::unordered_map<RightKeyType, JoinedType>& right_map,
                 LeftKeyExtractor left_key_ex, LeftValueExtractor left_value_ex)
        : left_flow_it_(left_flow_it),
          left_flow_end_(left_flow_end),
          right_map_(&right_map),
          left_key_ex_(left_key_ex),
          left_value_ex_(left_value_ex) {
      UpdateResult();
    }

    JoinIterator(typename LeftFlowType::iterator left_flow_it,
                 typename LeftFlowType::const_iterator left_flow_end,
                 const std::unordered_map<RightKeyType, JoinedType>& right_map,
                 LeftKeyExtractor left_key_ex, LeftValueExtractor left_value_ex)
        : left_flow_it_(left_flow_it),
          left_flow_end_(left_flow_end),
          right_map_(&right_map),
          left_key_ex_(left_key_ex),
          left_value_ex_(left_value_ex) {
      UpdateResult();
    }

    JoinIterator(typename LeftFlowType::const_iterator left_flow_it,
                 typename LeftFlowType::iterator left_flow_end,
                 const std::unordered_map<RightKeyType, JoinedType>& right_map,
                 LeftKeyExtractor left_key_ex, LeftValueExtractor left_value_ex)
        : left_flow_it_(left_flow_it),
          left_flow_end_(left_flow_end),
          right_map_(&right_map),
          left_key_ex_(left_key_ex),
          left_value_ex_(left_value_ex) {
      UpdateResult();
    }

    JoinIterator(typename LeftFlowType::iterator left_flow_it,
                 typename LeftFlowType::iterator left_flow_end,
                 const std::unordered_map<RightKeyType, JoinedType>& right_map,
                 LeftKeyExtractor left_key_ex, LeftValueExtractor left_value_ex)
        : left_flow_it_(left_flow_it),
          left_flow_end_(left_flow_end),
          right_map_(&right_map),
          left_key_ex_(left_key_ex),
          left_value_ex_(left_value_ex) {
      UpdateResult();
    }

    reference operator*() const { return current_; }

    JoinIterator& operator++() {
      ++left_flow_it_;
      UpdateResult();
      return *this;
    }

    bool operator==(const JoinIterator& other) const {
      return left_flow_it_ == other.left_flow_it_;
    }
    bool operator!=(const JoinIterator& other) const {
      return !(*this == other);
    }

   private:
    void UpdateResult() {
      if (left_flow_it_ != left_flow_end_) {
        const auto& left_flow_item = *left_flow_it_;
        auto key = left_key_ex_(left_flow_item);
        auto it = right_map_->find(key);

        current_ = {left_value_ex_(left_flow_item),
                    it != right_map_->end()
                        ? std::optional<JoinedType>{it->second}
                        : std::nullopt};
      }
    }

    typename LeftFlowType::const_iterator left_flow_it_;
    typename LeftFlowType::const_iterator left_flow_end_;
    const std::unordered_map<RightKeyType, JoinedType>* right_map_;
    LeftKeyExtractor left_key_ex_;
    LeftValueExtractor left_value_ex_;
    ResultType current_;
  };

  using const_iterator = JoinIterator;

  JoinIterator begin() const {
    return JoinIterator(left_flow_.begin(), left_flow_.end(), right_map_,
                        left_key_ex_, left_value_ex_);
  }

  JoinIterator end() const {
    return JoinIterator(left_flow_.end(), left_flow_.end(), right_map_,
                        left_key_ex_, left_value_ex_);
  }

 private:
  void BuildRightMap() {
    for (const auto& item : right_) {
      auto key = right_key_ex_(item);
      auto value = right_value_ex_(item);
      right_map_.emplace(key, value);
    }
  }

  LeftFlow left_flow_;
  RightFlow right_;
  LeftKeyExtractor left_key_ex_;
  RightKeyExtractor right_key_ex_;
  LeftValueExtractor left_value_ex_;
  RightValueExtractor right_value_ex_;
  std::unordered_map<RightKeyType, JoinedType> right_map_;
};