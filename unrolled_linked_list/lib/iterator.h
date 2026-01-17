#pragma once
#include <cstddef>
#include <exception>

#include "node.h"

template <typename T, size_t NodeMaxSize = 10,
typename Allocator = std::allocator<T>>

class Iterator {
 public:
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using reference = T&;
  using iterator_category = std::bidirectional_iterator_tag;

  Iterator(Node<T, NodeMaxSize, Allocator>* node, size_t ind) {
    node_ = node;
    index_ = ind;
  }

  Node<T, NodeMaxSize, Allocator>* get_node() const {
    return node_;
  }

  size_t get_index() const {
    return index_;
  }

  T& operator*() { 
    return *node_->get(index_);
  }

  T* operator->() { 
    return node_->get(index_);
  }

  Iterator<T, NodeMaxSize, Allocator> operator++() {
    if (node_) {
      if (++index_ >= node_->count_) {
        node_ = node_->next_;
        index_ = 0;
      }
    }
    return *this;
  }

  Iterator<T, NodeMaxSize, Allocator> operator++(int) {
    Iterator<T, NodeMaxSize, Allocator> copy = *this;
    ++(*this);
    return copy;
  }

  Iterator<T, NodeMaxSize, Allocator> operator--() {
    if (node_) {
      if (index_ == 0) {
        node_ = node_->prev_;
        if (node_) {
          index_ = node_->count_ - 1;
        } else {
          index_ = 0;
        }
      } else {
        --index_;
      }
    }
    return *this;
  }

  Iterator<T, NodeMaxSize, Allocator> operator--(int) {
    Iterator<T, NodeMaxSize, Allocator> copy = *this;
    --(*this);
    return copy;
  }

  bool operator==(const Iterator& other) const {
    return (node_ == other.node_) && (index_ == other.index_);
  }

  bool operator!=(const Iterator& other) const {
    return !(*this == other);
  }

 private:
  Node<T, NodeMaxSize, Allocator>* node_;
  size_t index_;
};