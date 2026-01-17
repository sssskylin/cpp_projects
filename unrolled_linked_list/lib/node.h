#pragma once
#include <memory>

template <typename T, size_t NodeMaxSize = 10,
          typename Allocator = std::allocator<T>>
struct Node {
  using allocator_type =
      typename std::allocator_traits<Allocator>::template rebind_alloc<T>;
  using allocator_traits = std::allocator_traits<allocator_type>;

  T* data_;
  size_t count_;
  Node* next_;
  Node* prev_;
  allocator_type allocator_;

  Node(const allocator_type& alloc = allocator_type()) {
    count_ = 1;
    next_ = nullptr;
    prev_ = nullptr;
    allocator_ = alloc;
    data_ = static_cast<T*>(std::malloc(NodeMaxSize * sizeof(T)));
  }

  ~Node() { std::free(data_); }

  Node(const Node&) = delete;
  Node& operator=(const Node&) = delete;

  T* get(size_t index) { return data_ + index; }

  const T* get(size_t index) const { return data_ + index; }
};