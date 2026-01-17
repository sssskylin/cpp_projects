#pragma once
#include <cstddef>
#include <exception>

#include "node.h"
#include "iterator.h"

template <typename T, size_t NodeMaxSize = 10,
typename Allocator = std::allocator<T>>

class ConstIterator {
 public:
  using value_type = T;
  using pointer = T*;
  using reference = T&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;

  ConstIterator(Node<T, NodeMaxSize, Allocator>* node, size_t ind) {
    node_ = node;
    index_ = ind;
  }

  ConstIterator(const Iterator<T, NodeMaxSize, Allocator> &other) {
    node_ = other.get_node();
    index_ = other.get_index();
  }

  Node<T, NodeMaxSize, Allocator>* get_node() {
    return node_;
  }

  size_t get_index() {
    return index_;
  }

  const T& operator*() const { 
    return *node_->get(index_);
  }

  const T* operator->() const { 
    return node_->get(index_);
  }

  ConstIterator<T, NodeMaxSize, Allocator> operator++() {
    if (node_) {
      if (++index_ >= node_->count_) {
        node_ = node_->next_;
        index_ = 0;
      }
    }
    return *this;
  }

  ConstIterator<T, NodeMaxSize, Allocator> operator++(int) {
    ConstIterator<T, NodeMaxSize, Allocator> copy = *this;
    ++(*this);
    return copy;
  }

  ConstIterator<T, NodeMaxSize, Allocator> operator--() {
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

  ConstIterator<T, NodeMaxSize, Allocator> operator--(int) {
    ConstIterator<T, NodeMaxSize, Allocator> copy = *this;
    --(*this);
    return copy;
  }

  bool operator==(const ConstIterator& other) const {
    return (node_ == other.node_) && (index_ == other.index_);
  }

  bool operator!=(const ConstIterator& other) const {
    return !(*this == other);
  }

 private:
  Node<T, NodeMaxSize, Allocator>* node_;
  size_t index_;
};