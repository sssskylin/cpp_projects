#pragma once
#include <type_traits>

template <typename StreamType, typename DelimiterType, typename FlowType>
class WriteAdapter {
 public:
  using value_type = typename std::decay_t<FlowType>::value_type;

  WriteAdapter(StreamType& stream, const DelimiterType& delimiter,
               const FlowType& flow)
      : stream_(stream), delimiter_(delimiter), flow_(flow) {
        auto it = flow_.begin();
    while (it != flow_.end()) {
      stream_ << *it;
      stream_ << delimiter_;
      ++it;
    }
  }

 private:
  StreamType& stream_;
  const DelimiterType& delimiter_;
  const FlowType& flow_;
};