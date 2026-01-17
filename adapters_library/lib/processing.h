#pragma once
#include <aggregate_by_key.h>
#include <dataflow.h>
#include <dir.h>
#include <filter.h>
#include <join.h>
#include <open_files.h>
#include <split.h>
#include <split_expected.h>
#include <transform.h>
#include <write.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

template <typename Container>
inline auto AsDataFlow(Container& input) {
  return DataFlow<Container>(input);
}

inline auto AsVector() {
  return [](const auto& flow) {
    using value_type = typename std::decay_t<decltype(*(flow.begin()))>;
    return std::vector<value_type>(flow.begin(), flow.end());
  };
}

inline auto Dir(const std::filesystem::path& path, bool recursive = false) {
  return DirAdapter(path, recursive);
}

inline auto OpenFiles() {
  return [](const auto& flow) { return OpenFilesAdapter(flow | AsVector()); };
}

inline auto Split(const std::string& delimiter) {
  return [delimiter](const auto& flow) {
    using FlowType = std::decay_t<decltype(flow)>;
    return SplitAdapter<FlowType>(delimiter, flow);
  };
}

template <typename Function>
inline auto Transform(Function func) {
  return [func](const auto& flow) {
    using FlowType = std::decay_t<decltype(flow)>;
    return TransformAdapter<Function, FlowType>(func, flow);
  };
}

template <typename Function>
inline auto Filter(Function func) {
  return [func](const auto& flow) {
    using FlowType = std::decay_t<decltype(flow)>;
    return FilterAdapter<Function, FlowType>(func, flow);
  };
}

template <typename RightFlow, typename LeftKeyExtractor,
          typename RightKeyExtractor, typename LeftValueExtractor,
          typename RightValueExtractor>
auto Join(RightFlow right, LeftKeyExtractor l_key_ex,
          RightKeyExtractor r_key_ex, LeftValueExtractor l_val_ex,
          RightValueExtractor r_val_ex) {
  return [=](const auto& left) {
    return JoinAdapter(left, right, l_key_ex, r_key_ex, l_val_ex, r_val_ex);
  };
}

template <typename RightFlow, typename LeftKeyExtractor,
          typename RightKeyExtractor>
auto Join(RightFlow right, LeftKeyExtractor l_key_ex,
          RightKeyExtractor r_key_ex) {
  return Join(
      right, l_key_ex, r_key_ex, [](const auto& item) { return item; },
      [](const auto& item) { return item; });
}

template <typename RightFlow>
auto Join(RightFlow right) {
  return Join(
      right, [](const auto& kv) { return kv.key; },
      [](const auto& kv) { return kv.key; },
      [](const auto& kv) { return kv.value; },
      [](const auto& kv) { return kv.value; });
}

template <typename T>

concept IsPair = requires(T t) {
  t.first;
  t.second;
};

template <typename Flow>
concept MapConcept = requires(Flow f){
  {*f.begin()} -> IsPair;
};

template <MapConcept MapFlow>
auto Join(MapFlow right) {
  return Join(
      right, [](const auto& kv) { return kv.first; },
      [](const auto& kv) { return kv.first; },
      [](const auto& kv) { return kv.second; },
      [](const auto& kv) { return kv.second; });
}


inline auto DropNullopt() {
  return Filter([](auto x) { return x != std::nullopt; });
}

inline auto SplitExpected() {
  return [](const auto& flow) {
    using FlowType = std::decay_t<decltype(flow)>;
    SplitExpectedAdapter<FlowType> split_expected(flow);
    return split_expected.GetPair();
  };
}

template <typename Accumulator, typename Aggregator, typename KeyFunc>
auto AggregateByKey(Accumulator init_value, Aggregator aggregator,
                    KeyFunc key_func) {
  return [init_value, aggregator, key_func](const auto& container) {
    return AggregateByKeyAdapter<std::decay_t<decltype(container)>, KeyFunc,
                                 Aggregator, Accumulator>(container, init_value,
                                                          aggregator, key_func);
  };
}

template <typename StreamType, typename DelimiterType>
inline auto Write(StreamType& stream, const DelimiterType& delimiter) {
  return [&stream, &delimiter](const auto& flow) {
    using FlowType = std::decay_t<decltype(flow)>;
    return WriteAdapter<StreamType, DelimiterType, FlowType>(stream, delimiter,
                                                             flow);
  };
}

template <typename StreamType>
inline auto Out(StreamType& stream) {
  return Write(stream, "");
}
template <typename Flowtype, typename Adapter>
auto operator|(const Flowtype& flow, const Adapter& adapter) {
  return adapter(flow);
}