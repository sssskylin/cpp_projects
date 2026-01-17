#pragma once

#include <fstream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

class OpenFilesAdapter {
 public:
  using value_type = std::stringstream;
  using pointer = std::shared_ptr<value_type>;

  explicit OpenFilesAdapter(const std::vector<std::filesystem::path>& paths)
      : path_(paths) {}

  class OpenFilesIterator {
   public:
    using iterator_category = std::input_iterator_tag;
    using value_type = value_type;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using PathIterator = std::vector<std::filesystem::path>::const_iterator;

    OpenFilesIterator() = default;

    OpenFilesIterator(PathIterator it, PathIterator end) : it_(it), end_(end) {
      OpenNextFile();
    }

    OpenFilesIterator& operator++() {
      ++it_;
      OpenNextFile();
      return *this;
    }

    reference operator*() const { return *current_file_; }

    bool operator==(const OpenFilesIterator& other) const {
      return it_ == other.it_;
    }
    bool operator!=(const OpenFilesIterator& other) const {
      return it_ != other.it_;
    }

   private:
    void OpenNextFile() {
      current_file_.reset();

      if (it_ == end_) {
        return;
      }

      std::ifstream file(*it_);
      if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + it_->string());
      }

      std::stringstream buffer;
      buffer << file.rdbuf();

      current_file_ = std::make_shared<value_type>(buffer.str());
    }

    PathIterator it_;
    PathIterator end_;
    std::shared_ptr<value_type> current_file_;
  };

  using iterator = OpenFilesIterator;
  using const_iterator = OpenFilesIterator;

  auto begin() const { return OpenFilesIterator(path_.begin(), path_.end()); }
  auto end() const { return OpenFilesIterator(path_.end(), path_.end()); }

 private:
  const std::vector<std::filesystem::path>& path_;
};
