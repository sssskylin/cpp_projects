#pragma once
#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

template <typename FlowType>
class SplitAdapter {
 public:
  using value_type = std::string;

  SplitAdapter(const std::string& delimiters, const FlowType& flow)
      : delimiters_(delimiters), flow_(flow) {}

  class SplitIterator {
   public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::string;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using FlowIterator =
        typename std::decay_t<FlowType>::const_iterator;

    SplitIterator(const FlowIterator& it, const FlowIterator& end,
                  const std::string& delimiters)
        : it_(it), end_(end), delimiters_(delimiters) {
      if (it_ != end_) {
        current_value_ = (*it_).str();
        ProcessNextDelimitedPart();
      }
    }

    reference operator*() const { return current_symbol_; }

    SplitIterator& operator++() {
      ProcessNextDelimitedPart();
      return *this;
    }

    bool operator==(const SplitIterator& other) const {
      return it_ == other.it_ && current_value_ == other.current_value_;
    }
    bool operator!=(const SplitIterator& other) const {
      return !(*this == other);
    }

   private:
    void ProcessNextDelimitedPart() {
      while (it_ != end_ || !current_value_.empty()) {
        size_t start = current_value_.find_first_not_of(' ');
        if (start != std::string::npos) {
          current_value_ = current_value_.substr(start);
        } else {
          current_value_.clear();
        }

        size_t pos = current_value_.find_first_of(delimiters_);

        if (pos != std::string::npos) {
          current_symbol_ = current_value_.substr(0, pos);

          size_t next = current_value_.find_first_not_of(' ', pos + 1);
          if (next != std::string::npos) {
            current_value_ = current_value_.substr(next);
          } else {
            current_value_.clear();
          }
          return;
        } else if (!current_value_.empty()) {
          current_symbol_ = current_value_;
          current_value_.clear();
          return;
        }

        ++it_;
        if (it_ != end_) {
          current_value_ = (*it_).str();
        } else {
          current_value_.clear();
          break;
        }
      }

      current_symbol_.clear();
    }

    FlowIterator it_;
    FlowIterator end_;
    std::string delimiters_;
    std::string current_value_;
    std::string current_symbol_;
  };

  using const_iterator = SplitIterator;

  SplitIterator begin() const {
    return SplitIterator(flow_.begin(), flow_.end(), delimiters_);
  }

  SplitIterator end() const {
    return SplitIterator(flow_.end(), flow_.end(), delimiters_);
  }

 private:
  const FlowType& flow_;
  std::string delimiters_;
};