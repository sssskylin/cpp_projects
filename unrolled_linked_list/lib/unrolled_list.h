#pragma once
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>

#include "const_iterator.h"
#include "iterator.h"

template <typename T, size_t NodeMaxSize = 10,
          typename Allocator = std::allocator<T>>
class unrolled_list {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using iterator = class Iterator<T, NodeMaxSize, Allocator>;
  using const_iterator = class ConstIterator<T, NodeMaxSize, Allocator>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using allocator_type = Allocator;
  using node_allocator_type = typename std::allocator_traits<
      Allocator>::template rebind_alloc<Node<T, NodeMaxSize, Allocator>>;
  using node_allocator_traits = std::allocator_traits<node_allocator_type>;

  unrolled_list(const allocator_type& alloc = allocator_type()) {
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
    allocator_ = alloc;
    node_allocator_ = alloc;
  }
  unrolled_list(const unrolled_list& other,
                const allocator_type& alloc = allocator_type()) {
    allocator_ = other.allocator_;
    node_allocator_ = other.node_allocator_;
    try {
      for (const value_type& value : other) {
        push_back(value);
      }
    } catch (...) {
      clear();
      throw std::runtime_error("Error during construction");
    }
  }
  unrolled_list(size_type n, const value_type& value) {
    for (size_type i = 0; i < n; ++i) {
      push_back(value);
    }
  }
  unrolled_list(std::initializer_list<value_type> list) {
    for (const value_type& value : list) {
      push_back(value);
    }
  }
  template <typename Iterator>
  unrolled_list(Iterator it_begin, Iterator it_end,
                const allocator_type& alloc = allocator_type()) noexcept(false) {
    allocator_ = alloc;
    node_allocator_ = alloc;
    Node<T, NodeMaxSize, Allocator>* current = nullptr;
    try {
      for (; it_begin != it_end; ++it_begin) {
        if (!current || current->count_ == NodeMaxSize) {
          Node<T, NodeMaxSize, Allocator>* new_node =
              node_allocator_traits::allocate(node_allocator_, 1);
          node_allocator_traits::construct(node_allocator_, new_node,
                                           allocator_);
          new_node->count_ = 0;
          if (current) {
            current->next_ = new_node;
            new_node->prev_ = current;
          } else {
            head_ = new_node;
          }
          current = new_node;
        }
        std::allocator_traits<allocator_type>::construct(
            allocator_, current->get(current->count_), *it_begin);
        ++current->count_;
        ++size_;
      }
    } catch (...) {
      clear();
      throw std::runtime_error("Error during construction");
    }
  }

  iterator begin() { return iterator(head_, 0); }
  iterator end() { return iterator(nullptr, 0); }
  const_iterator begin() const { return iterator(head_, 0); }
  const_iterator end() const { return iterator(nullptr, 0); }
  const_iterator cbegin() const { return begin(); }
  const_iterator cend() const { return end(); }
  reverse_iterator rbegin() { return reverse_iterator(begin()); }
  reverse_iterator rend() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(begin());
  }
  const_reverse_iterator rend() const { return const_reverse_iterator(end()); }
  const_reverse_iterator crbegin() const { return rbegin(); }
  const_reverse_iterator crend() const { return rend(); }

  size_t size() const { return size_; }
  size_t max_size() const {
    return std::numeric_limits<size_type>::max() / sizeof(T);
  }
  bool empty() const { return head_ == nullptr; }

  bool operator==(const unrolled_list& other) const {
    if (size_ != other.size_) {
      return false;
    }

    iterator it = begin();
    iterator it_other = other.begin();

    while (it != end() && it_other != other.end()) {
      if (*it != *it_other) {
        return false;
      }
      ++it;
      ++it_other;
    }

    return true;
  }
  bool operator!=(const unrolled_list& other) const {
    return !(*this == other);
  }

  allocator_type get_allocator() const { return allocator_; }

  reference front() { return *head_->get(0); }
  const_reference front() const { return *head_->get(0); }
  reference back() { return *tail_->get(tail_->count_ - 1); }
  const_reference back() const { return *tail_->get(tail_->count_ - 1); }

  void push_back(const T& value) {
    if (!tail_ || tail_->count_ == NodeMaxSize) {
      Node<T, NodeMaxSize, Allocator>* new_node =
          node_allocator_traits::allocate(node_allocator_, 1);
      node_allocator_traits::construct(node_allocator_, new_node, allocator_);

      new_node->count_ = 0;
      if (tail_) {
        tail_->next_ = new_node;
        new_node->prev_ = tail_;
      } else {
        head_ = new_node;
      }
      tail_ = new_node;
    }
    std::allocator_traits<allocator_type>::construct(
        allocator_, tail_->get(tail_->count_), value);
    ++tail_->count_;
    ++size_;
  }
  void pop_back() noexcept {
    if (!tail_) {
      return;
    }
    --tail_->count_;
    --size_;
    if (tail_->count_ == 0) {
      Node<T, NodeMaxSize, Allocator>* old_tail = tail_;
      if (tail_->prev_) {
        tail_ = tail_->prev_;
        tail_->next_ = nullptr;
      } else {
        head_ = nullptr;
        tail_ = nullptr;
      }
      node_allocator_traits::destroy(node_allocator_, old_tail);
      node_allocator_traits::deallocate(node_allocator_, old_tail, 1);
    }
  }
  void push_front(const T& value) {
    if (!head_ || head_->count_ == NodeMaxSize) {
      Node<T, NodeMaxSize, Allocator>* new_node =
          node_allocator_traits::allocate(node_allocator_, 1);
      node_allocator_traits::construct(node_allocator_, new_node, allocator_);
      new_node->next_ = head_;
      new_node->count_ = 0;
      if (head_) {
        head_->prev_ = new_node;
      } else {
        tail_ = new_node;
      }
      head_ = new_node;
    }

    if (head_->count_ > 0) {
      for (int i = head_->count_; i > 0; --i) {
        std::allocator_traits<allocator_type>::construct(
            allocator_, head_->get(i), *head_->get(i - 1));
        std::allocator_traits<allocator_type>::destroy(allocator_,
                                                       head_->get(i - 1));
      }
    }
    std::allocator_traits<allocator_type>::construct(allocator_, head_->get(0), value);
    ++head_->count_;
    ++size_;
  }
  void pop_front() noexcept {
    if (!head_) {
      return;
    }
    if (head_->count_ > 1) {
      for (int i = 0; i < head_->count_ - 1; ++i) {
        std::allocator_traits<allocator_type>::construct(
            allocator_, head_->get(i), *head_->get(i + 1));
        std::allocator_traits<allocator_type>::destroy(allocator_,
                                                       head_->get(i + 1));
      }
    } else if (head_->count_ == 1) {
      std::allocator_traits<allocator_type>::destroy(allocator_, head_->get(0));
    }

    --head_->count_;
    --size_;
    if (head_->count_ == 0) {
      Node<T, NodeMaxSize, Allocator>* old_head = head_;
      if (head_->next_) {
        head_ = head_->next_;
        head_->prev_ = nullptr;
      } else {
        head_ = nullptr;
        tail_ = nullptr;
      }
      node_allocator_traits::destroy(node_allocator_, old_head);
      node_allocator_traits::deallocate(node_allocator_, old_head, 1);
    }
  }
  iterator insert(const_iterator pos, const T& value) {
    iterator res(pos.get_node(), pos.get_index());
    if (!res.get_node()) {
      push_back(value);
      return iterator(tail_, tail_->count_ - 1);
    }

    if (res.get_node()->count_ < NodeMaxSize) {
      for (int i = res.get_node()->count_; i > res.get_index(); --i) {
        std::allocator_traits<allocator_type>::construct(
            allocator_, res.get_node()->get(i), *res.get_node()->get(i - 1));
        std::allocator_traits<allocator_type>::destroy(
            allocator_, res.get_node()->get(i - 1));
      }
      std::allocator_traits<allocator_type>::construct(
          allocator_, res.get_node()->get(res.get_index()), value);
      ++res.get_node()->count_;
      ++size_;
      return res;
    }

    Node<T, NodeMaxSize, Allocator>* new_node =
        node_allocator_traits::allocate(node_allocator_, 1);
    node_allocator_traits::construct(node_allocator_, new_node, allocator_);
    new_node->next_ = res.get_node()->next_;
    new_node->prev_ = res.get_node();

    if (res.get_node()->next_) {
      res.get_node()->next_->prev_ = new_node;
    } else {
      tail_ = new_node;
    }
    res.get_node()->next_ = new_node;

    size_t move_count = res.get_node()->count_ - res.get_index();
    for (int i = 0; i < move_count; ++i) {
      std::allocator_traits<allocator_type>::construct(
          allocator_, new_node->get(i),
          *res.get_node()->get(res.get_index() + i));
      std::allocator_traits<allocator_type>::destroy(
          allocator_, res.get_node()->get(res.get_index() + i));
    }
    new_node->count_ = move_count;
    res.get_node()->count_ = res.get_index();

    std::allocator_traits<allocator_type>::construct(
        allocator_, res.get_node()->get(res.get_index()), value);
    ++res.get_node()->count_;
    ++size_;

    return res;
  }
  iterator insert(const_iterator pos, size_t n, const T& value) {
    if (n == 0) {
      return iterator(pos.get_node(), pos.get_index());
    }

    iterator res(pos.get_node(), pos.get_index());
    for (size_type i = 0; i < n; ++i) {
      res = insert(res, value);
    }
    return res;
  }
  iterator erase(const_iterator pos) noexcept {
    Node<T, NodeMaxSize, Allocator>* node = pos.get_node();
    size_t index = pos.index;

    std::allocator_traits<allocator_type>::destroy(allocator_,
                                                   node->get(index));

    for (int i = index; i < node->count_ - 1; ++i) {
      std::allocator_traits<allocator_type>::construct(allocator_, node->get(i),
                                                       *node->get(i + 1));
      std::allocator_traits<allocator_type>::destroy(allocator_,
                                                     node->get(i + 1));
    }

    --node->count_;
    --size_;

    if (node->count_ == 0) {
      if (node->prev_) {
        node->prev_->next_ = node->next_;
      } else {
        head_ = node->next_;
      }

      if (node->next_) {
        node->next_->prev_ = node->prev_;
      } else {
        tail_ = node->prev_;
      }

      Node<T, NodeMaxSize, Allocator>* next = node->next_;
      node_allocator_traits::destroy(node_allocator_, node);
      node_allocator_traits::deallocate(node_allocator_, node, 1);

      if (next) {
        return iterator(next, 0);
      } else {
        return end();
      }
    }

    if (index < node->count_) {
      return iterator(node, index);
    } else if (node->next_) {
      return iterator(node->next_, 0);
    } else {
      return end();
    }
  }
  iterator erase(const_iterator pos, const_iterator pos2) noexcept {
    while (pos != pos2) {
      pos = erase(pos);
    }
    return iterator(pos.get_node(), pos.get_index());
  }
  void clear() noexcept {
    Node<T, NodeMaxSize, Allocator>* current = head_;
    while (current) {
      Node<T, NodeMaxSize, Allocator>* next_ = current->next_;
      node_allocator_traits::destroy(node_allocator_, current);
      node_allocator_traits::deallocate(node_allocator_, current, 1);
      current = next_;
    }
    head_ = tail_ = nullptr;
    size_ = 0;
  }

 private:
  Node<T, NodeMaxSize, Allocator>* head_;
  Node<T, NodeMaxSize, Allocator>* tail_;
  size_t size_;
  allocator_type allocator_;
  node_allocator_type node_allocator_;
};