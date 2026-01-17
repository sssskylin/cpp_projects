#pragma once
#include <filesystem>
#include <optional>
#include <typeinfo>
#include <variant>
#include <iostream>

class DirAdapter {
 public:
  using value_type = std::filesystem::path;
  class DirIterator {
   public:
    using iterator_category = std::input_iterator_tag;
    using value_type = value_type;
    using difference_type = std::ptrdiff_t;
    using RecursiveIterator = std::filesystem::recursive_directory_iterator;
    using Iterator = std::filesystem::directory_iterator;
    using pointer = const value_type*;
    using reference = const value_type&;

    DirIterator(bool recursive, const value_type& path = "") {
      if (recursive) {
        it_ = RecursiveIterator(path);
        end_ = RecursiveIterator();
      } else {
        it_ = Iterator(path);
        end_ = Iterator();
      }
      PassToValidDirectory();
    };

    DirIterator& operator++() {
      if (std::holds_alternative<Iterator>(it_)) {
        if (std::get<Iterator>(it_) != std::get<Iterator>(end_)) {
          ++std::get<Iterator>(it_);
        }
      } else if (std::get<RecursiveIterator>(it_) !=
                 std::get<RecursiveIterator>(end_)) {
        ++std::get<RecursiveIterator>(it_);
      }
      PassToValidDirectory();
      return *this;
    }

    value_type operator*() const {
      if (std::holds_alternative<Iterator>(it_)) {
        if (std::get<Iterator>(it_) != std::get<Iterator>(end_)) {
          return std::get<Iterator>(it_)->path();
        }
      }
      return std::get<RecursiveIterator>(it_)->path();
    }

    bool operator==(const DirIterator& other) const { return other.it_ == it_; }

    bool operator!=(const DirIterator& other) const {
      return !(*this == other);
    }

   private:
    void PassToValidDirectory() {
      if (std::holds_alternative<Iterator>(it_)) {
        while (std::get<Iterator>(it_) != std::get<Iterator>(end_) &&
               std::get<Iterator>(it_)->is_directory()) {
          ++std::get<Iterator>(it_);
        }
      } else {
        while (std::get<RecursiveIterator>(it_) !=
                   std::get<RecursiveIterator>(end_) &&
               std::get<RecursiveIterator>(it_)->is_directory()) {
          ++std::get<RecursiveIterator>(it_);
        }
      }
    }

    std::variant<RecursiveIterator, Iterator> it_;
    std::variant<RecursiveIterator, Iterator> end_;
  };

  using const_iterator = DirIterator;

  DirAdapter(const std::filesystem::path& path, bool recursive = false)
      : it_(recursive, path), end_(recursive) {};

  const_iterator begin() const { return it_; }
  const_iterator end() const { return end_; }

  bool operator==(const DirAdapter& other) const { return it_ == other.it_; };
  bool operator!=(const DirAdapter& other) const { return !(*this == other); }

 private:
  DirIterator it_;
  DirIterator end_;
};